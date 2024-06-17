#include "webserver.h"

/**
 * @brief Constructor for the webserver class.
 * 
 * Initializes a web server with a specified ID.
 * 
 * @param id ID of the web server.
 */
webserver::webserver(int id): id(id){
    
}

/**
 * @brief Processes a request received by the web server.
 * 
 * Simulates processing of a request by logging the request details,
 * waiting for a simulated processing time, and then logging completion.
 * 
 * @param req The request to be processed.
 */
void webserver::proccess_req(request req){
    // switch case here
    std::ofstream x;
    x.open("logs.txt", std::ofstream::app);
    x << "server: " << id << " received a request! IP in: " << req.IP_in <<", the task will take: " << req.length << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(req.length));
    x << "server: " << id << " finished a request! IP out: " << req.IP_out << std::endl;
}
