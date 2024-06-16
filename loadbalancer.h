#include "webserver.h"

class loadbalancer{

    public: 
        loadbalancer();
        void addRequestsThread();
        void webserverThread(int server_id);
        void runLoadBalancer();
        void roundRobin();
        request generateRequest();
    private:
        int cur_num_servers;
        int max_num_servers;
        //vector of webservers
};
