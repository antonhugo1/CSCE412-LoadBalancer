#ifndef LOADBALANCER_H
#define LOADBALANCER_H
#include "webserver.h"

class loadbalancer{

    public: 

        loadbalancer(int num_cycles, int num_servers);
 
        void addRequestsThread();

        void webserverThread(int server_id);

        void runLoadBalancer();

        void loop();

        request generateRequest();
    
    private:
        int num_cycles;                      ///< Number of cycles (iterations) the load balancer will run.
        int num_servers;                     ///< Number of servers in the load balancer.
        std::queue<request> request_queue;   ///< Queue to hold incoming requests.
        std::condition_variable cv;          ///< Condition variable for thread synchronization.
        std::mutex cv_m;                     ///< Mutex for protecting shared resources.
        bool add_requests = false;           ///< Flag indicating whether new requests are being added.
        std::queue<int> inactive_servers;    ///< Queue of inactive server IDs.
        std::vector<bool> active_servers;    ///< Vector indicating the availability of each server.
        std::atomic_int counter = 0;         ///< Atomic counter to track the number of cycles processed.
        std::atomic_bool stop = false;       ///< Atomic flag to indicate whether the load balancer should stop.
        std::ofstream log_file;              ///< Output stream for logging operations.
        //vector of webservers
};
#endif // LOADBALANCER_H
