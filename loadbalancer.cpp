#include "loadbalancer.h"
#include <stdlib.h>
#include <chrono>
#include <atomic>
// #include <ofstream>
using namespace std;

std::queue<request> request_queue;
condition_variable cv;
mutex cv_m;
bool add_requests = false;
queue<int> inactive_servers;
vector<bool> active_servers;
int counter = 0;

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
void loadbalancer::roundRobin(){
    while(true){
        unique_lock<mutex> ul (cv_m);
        cv.wait(ul, [&]{return !request_queue.empty();});
        if(!inactive_servers.empty()){
            // cout<<"hi";
            int next_server = inactive_servers.front();
            cout << "next server: " << next_server << endl;
            active_servers.at(next_server) = true;
            inactive_servers.pop();
            // inactive_servers.push(next_server);
        }
    }



}
request loadbalancer::generateRequest(){
    int ip_in1 = (rand() % 256);
    string ip_in = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    string ip_out = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    int req_type = rand() % 3;
    string inputs[4] = {"asdlkASDAkajdl", "ALKSALKJsdsdAKLSKJD", "ASKDJL", "elena gharipour"};
    string input = inputs[rand() % 4];
    return request(ip_in, ip_out, req_type, input);
}
void loadbalancer::webserverThread(int server_id){
    // lock 
    webserver server(server_id);

    while(true){
        unique_lock<mutex> ul (cv_m);
        cv.wait(ul, [&]{return active_servers.at(server_id);});
        if(!request_queue.empty()){
            request_queue.pop();
            cout << "server: " << server_id << " got a request! queue size: "<< request_queue.size() << endl;
        }
        if(request_queue.empty()){
            // cout << "empty queue!" << endl;
            add_requests = false;
        }
        active_servers.at(server_id) = false;
    }

}


void loadbalancer::runLoadBalancer(){

    // if queue is not empty -> assign tasks i.e. pop queue
    vector<thread> vec_threads;
    int number_of_servers = 100;
    vector<atomic<bool>> signals(number_of_servers);
    for(size_t i = 0; i < 10000; i++){
        request_queue.push(generateRequest());
    }
    for(size_t i = 0; i < number_of_servers; i++){
        inactive_servers.push(i);
        active_servers.push_back(true);
    }
    for(size_t i = 0; i < number_of_servers; i++){
        vec_threads.emplace_back(&loadbalancer::webserverThread, this,  i);
    }
    thread x(&loadbalancer::addRequestsThread, this);
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
            break;
        }
    }  

    for(size_t i = 0; i < number_of_servers; i++){
        vec_threads.at(i).join();
    }
}