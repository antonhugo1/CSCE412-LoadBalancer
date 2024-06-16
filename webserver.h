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
struct request{
    std::string IP_in;
    std::string IP_out;
    int length;
    request(std::string IP_in, std::string IP_out, int length):
    IP_in(IP_in), IP_out(IP_out), length(length){

    }

};

class webserver{

    public:
        webserver(int id);
        void proccess_req(request req);
    private:
        int id;
};