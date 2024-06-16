#include <string>
#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
// request struct
struct request{
    std::string IP_in;
    std::string IP_out;
    int request_type;
    std::string input;
    request(std::string IP_in, std::string IP_out, int request_type, std::string input):
    IP_in(IP_in), IP_out(IP_out), request_type(request_type), input(input){

    }

};

class webserver{

    public:
        webserver(int id);
        void proccess_req(int request_type);
        void arbit_func();
    private:
        int id;
};