#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <iostream>
#include <queue>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdlib.h>
#include <atomic>

// request struct
/**
 * @brief Represents a network request.
 * 
 * Contains the source IP address, destination IP address, and length of the request.
 */
struct request {
    std::string IP_in;      ///< Source IP address of the request.
    std::string IP_out;     ///< Destination IP address of the request.
    int length;             ///< Length of the request.
    
    /**
     * @brief Constructor for the request struct.
     * 
     * @param IP_in Source IP address of the request.
     * @param IP_out Destination IP address of the request.
     * @param length Length of the request.
     */
    request(std::string IP_in, std::string IP_out, int length):
        IP_in(IP_in), IP_out(IP_out), length(length) {}
};

/**
 * @brief The webserver class represents a server that processes incoming requests.
 * 
 * Each webserver object handles requests assigned to it by the load balancer.
 */
class webserver {

public:
    webserver(int id);
    void proccess_req(request req);

private:
    int id;  ///< ID of the web server.
};

#endif // WEBSERVER_H
