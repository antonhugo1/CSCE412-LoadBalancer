#include "webserver.h"


webserver::webserver(int id): id(id){

}

void webserver::proccess_req(request req){
    // switch case here
    std::ofstream x;
    x.open("logs.txt", std::ofstream::app);
    x << "server: " << id << " received a request! IP in: " << req.IP_in <<", the task will take: " << req.length << " seconds." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(req.length));
    x << "server: " << id << " finished a request! IP out: " << req.IP_out << std::endl;

}
