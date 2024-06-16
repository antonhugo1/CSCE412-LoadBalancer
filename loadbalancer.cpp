#include "loadbalancer.h"
// #include <ofstream>
using namespace std;

std::queue<request> request_queue;
condition_variable cv;
mutex cv_m;
bool add_requests = false;
queue<int> inactive_servers;
vector<bool> active_servers;
atomic_int counter = 0;
atomic_bool stop = false;
ofstream log_file;

loadbalancer::loadbalancer(){
    std::cout << "hello world" << std::endl;
}

void loadbalancer::addRequestsThread(){
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
        unique_lock<mutex> ul (cv_m);
        int num_req = rand() % 1000;
        for(size_t i = 0; i < num_req; i++){
            request_queue.push(generateRequest());
        }
        cout << " pushed new request!" << endl;
        add_requests = true;
        cv.notify_all();

    }
}

request loadbalancer::generateRequest(){
    int ip_in1 = (rand() % 256);
    string ip_in = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    string ip_out = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    int length = (rand() % 3)+1;
    return request(ip_in, ip_out, length);
}
void loadbalancer::webserverThread(int server_id){
    // lock 
    webserver server(server_id);

    while(true){
        unique_lock<mutex> ul (cv_m);
        cv.wait(ul, [&]{return active_servers.at(server_id) || stop;});
        // cv.wait(ul);

        if(stop){
            break;
        }
        if(!request_queue.empty()){
            log_file << "server: " << server_id << " got a request! queue size: "<< request_queue.size() << endl;
            request_queue.pop();
            ul.unlock();
            server.proccess_req(request_queue.front()); 
        }
        ul.lock();
        log_file << "finished request! from server: " << server_id << endl;
        ul.unlock();
        if(request_queue.empty()){
            // cout << "empty queue!" << endl;
            add_requests = false;
        }
        active_servers.at(server_id) = false;
    }

}

void loadbalancer::loop(){
    while(true){
        // assign task here
        unique_lock<mutex> ul (cv_m);
        counter++;
        int next_available = inactive_servers.front();
        inactive_servers.pop();
        inactive_servers.push(next_available);
        active_servers.at(next_available) = true;
        cv.notify_all();
        if(counter == 10000){
            cout << "finished!" << endl;
            stop = true;
            cv.notify_all();
            log_file << "end queue size: " << request_queue.size() << endl;
            log_file.close();
            break;
        }
    }  
}

void loadbalancer::runLoadBalancer(){

    // if queue is not empty -> assign tasks i.e. pop queue
    ifstream make_logfile("logs.txt");
    if(!make_logfile.good()){
        ofstream mk_logfile("logs.txt");
        mk_logfile.open("logs.txt");
        mk_logfile.close();
    }
    log_file.open("logs.txt", ofstream::app);
    vector<thread> vec_threads;
    int number_of_servers = 100;
    vector<atomic<bool>> signals(number_of_servers);
    for(size_t i = 0; i < 1000; i++){
        request_queue.push(generateRequest());
    }
    log_file << "beginning queue size: " << request_queue.size() << endl;
    for(size_t i = 0; i < number_of_servers; i++){
        inactive_servers.push(i);
        active_servers.push_back(false);
    }
    for(size_t i = 0; i < number_of_servers; i++){
        vec_threads.emplace_back(&loadbalancer::webserverThread, this,  i);
    }
    thread x(&loadbalancer::addRequestsThread, this);
    thread y(&loadbalancer::loop, this);

    while(true){}

    for(size_t i = 0; i < number_of_servers; i++){
        vec_threads.at(i).join();
    }
}