#ifndef LOADBALANCER_H
#define LOADBALANCER_H
#include "webserver.h"

class loadbalancer{

    public: 
        /**
        * @brief Constructor for the loadbalancer class.
        * 
        * Initializes the load balancer with the specified number of cycles and servers.
        * 
        * @param num_cycles Number of cycles (iterations) the load balancer will run.
        * @param num_servers Number of servers in the load balancer.
        */
        loadbalancer(int num_cycles, int num_servers);
        /**
        * @brief Thread function that continuously adds requests to the request queue.
        * 
        * Randomly generates requests and adds them to the request queue at random intervals.
        * Notifies waiting threads when new requests are added.
        */
        void addRequestsThread();
        /**
        * @brief Thread function representing each web server in the load balancer.
        * 
        * Waits for notifications and processes requests assigned to a specific server ID.
        * Manages server availability dynamically.
        * 
        * @param server_id ID of the server this thread is managing.
        */
        void webserverThread(int server_id);
        /**
        * @brief Main function to start the load balancer.
        * 
        * Initializes logging, generates initial requests, starts server threads, and manages
        * the main loop of the load balancer.
        */
        void runLoadBalancer();
        /**
        * @brief Main loop of the load balancer.
        * 
        * Manages the assignment of tasks to servers based on their availability.
        * Tracks the number of cycles and stops processing when the specified number
        * of cycles is reached.
        */
        void loop();
        /**
        * @brief Generates a random request.
        * 
        * Randomly generates an IP address for input and output and a random length for a request.
        * 
        * @return A randomly generated request.
        */
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
