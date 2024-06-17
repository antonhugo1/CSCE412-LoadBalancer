#include "loadbalancer.h"

int main(){
    
    // create load balancer
    int num_servers = 0;
    int num_cycles = 0;
    std::cout << "How many servers would you like to run? ";
    std::cin >> num_servers;
    std::cout << std::endl;
    std::cout << "How many cycles will the load balancer run for? ";
    std::cin >> num_cycles;
    std::cout << std::endl;
    loadbalancer x(num_servers, num_cycles);
    x.runLoadBalancer();
    return 0;
    
}


// As of Jun 12 22:02
// goals:
// html code commenting with Doxygen
// logging
// video
