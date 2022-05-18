#include "rpc.h"
#include <iostream>


Rpc::Rpc(std::string ip,short port){
    this->ip = ip;
    this->port = port;
    json_list = json::object();
}

Rpc::~Rpc(){

}

void Rpc::funcname_to_json(std::string funcname){
    json_list.push_back({"funcname",funcname});
}
bool Rpc::handle_situation(){
    if(json_list["code"].get<int>() == 2){
        return true;
    }
    else{
        error_message = json_list["message"].get<std::string>();
    }
    return false;
}

int main(){
    Rpc temp("127.0.0.1",8080);
    auto s = temp.call<std::string>("appends","zmm","sb");
    std::cout<< s<<std::endl;
    return 0;
}