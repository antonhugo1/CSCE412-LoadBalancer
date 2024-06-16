#include "webserver.h"

class loadbalancer{

    public: 
        loadbalancer();
        void addRequestsThread();
        void webserverThread(int server_id);
        void runLoadBalancer();
        void loop();
        request generateRequest();
    private:
        //vector of webservers
};
