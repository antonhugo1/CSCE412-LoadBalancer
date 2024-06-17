#include "loadbalancer.h"
using namespace std;

/**
 * @brief Constructor for loadbalancer class.
 * 
 * Initializes the load balancer with the given number of servers and cycles.
 * Prints a hello world message upon initialization.
 * 
 * @param num_servers Number of servers in the load balancer.
 * @param num_cycles Number of cycles (iterations) the load balancer will run.
 */
loadbalancer::loadbalancer(int num_servers, int num_cycles): num_servers(num_servers), num_cycles(num_cycles){
    std::cout << "hello world" << std::endl;
}
/**
 * @brief Function that runs in a separate thread to add requests to the request queue.
 * 
 * Randomly generates requests and adds them to the request queue. Notifies waiting threads
 * when new requests are added.
 */
void loadbalancer::addRequestsThread(){
    // randomly adds requests and notifies all 
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 10));
        unique_lock<mutex> ul (cv_m);
        int num_req = rand() % 1000;
        for(size_t i = 0; i < num_req; i++){
            request_queue.push(generateRequest());
        }
        cout << "Pushed new requests!" << endl;
        add_requests = true;
        cv.notify_all();

    }
}
/**
 * @brief Generates a random request.
 * 
 * Randomly generates an IP address for input and output and a random length for a request.
 * 
 * @return A randomly generated request.
 */

request loadbalancer::generateRequest(){
    int ip_in1 = (rand() % 256);
    string ip_in = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    string ip_out = to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256) + "." + to_string(rand() % 256);
    int length = (rand() % 3)+1;
    return request(ip_in, ip_out, length);
}

/**
 * @brief Function that represents the thread for each web server.
 * 
 * Handles requests assigned to a specific server ID. Waits for notifications
 * and processes requests from the request queue.
 * 
 * @param server_id ID of the server this thread is managing.
 */
void loadbalancer::webserverThread(int server_id){
    // lock 
    webserver server(server_id);

    while(true){
        unique_lock<mutex> ul (cv_m);
        cv.wait(ul, [&]{return active_servers.at(server_id) || stop;});
        if(stop){
            break;
        }
        if(!request_queue.empty()){
            request_queue.pop();
            ul.unlock();
            server.proccess_req(request_queue.front()); 
        }
        if(request_queue.empty()){
            // cout << "empty queue!" << endl;
            add_requests = false;
        }
        active_servers.at(server_id) = false;
    }

}
/**
 * @brief Main loop of the load balancer.
 * 
 * Manages the assignment of tasks to servers based on their availability.
 * Tracks the number of cycles and stops processing when the specified number
 * of cycles is reached.
 */
void loadbalancer::loop(){
    while(true){
        // assign task here
        unique_lock<mutex> ul (cv_m);
        // cv.wait(ul, [&]{return add_requests;});

        counter++;
        int next_available = inactive_servers.front();
        inactive_servers.pop();
        inactive_servers.push(next_available);
        active_servers.at(next_available) = true;
        cv.notify_all();
        if(counter == num_cycles){
            cout << "finished!" << endl;
            stop = true;
            cv.notify_all();
            log_file << "end queue size: " << request_queue.size() << endl;
            log_file.close();
            break;
        }
    }  
}

/**
 * @brief Starts the load balancer.
 * 
 * Initializes logging, generates initial requests, starts server threads,
 * and manages the main loop of the load balancer.
 */
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
    vector<atomic<bool>> signals(num_servers);
    int num_requests = num_servers * 100;
    for(size_t i = 0; i < num_requests; i++){
        request_queue.push(generateRequest());
    }
    log_file << "beginning queue size: " << request_queue.size() << endl;
    
    for(size_t i = 0; i < num_servers; i++){
        inactive_servers.push(i);
        active_servers.push_back(false);
    }

    for(size_t i = 0; i < num_servers; i++){
        vec_threads.emplace_back(&loadbalancer::webserverThread, this,  i);
    }
    thread y(&loadbalancer::loop, this);

    thread x(&loadbalancer::addRequestsThread, this);

    while(true){
    }
    for(size_t i = 0; i < num_servers; i++){
        vec_threads.at(i).join();
    }

 
}