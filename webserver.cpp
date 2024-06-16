#include "webserver.h"


webserver::webserver(int id): id(id){

}

void webserver::proccess_req(request req){
    // switch case here

    std::this_thread::sleep_for(std::chrono::seconds(req.length));
    std::ofstream x;
    x.open("logs.txt", std::ofstream::app);

    x << "server: " << id << " finished a request!" << std::endl;

}
