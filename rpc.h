#include <hv/requests.h>
#include <vector>
#include <string>
#include <hv/json.hpp>
using json = nlohmann::json;

class Rpc
{
private:
    std::string ip;
    short port;
    json json_list;
    std::string error_message;
    void funcname_to_json(std::string funcname);

    template<typename Argument>
    void argument_to_json(Argument argument){
        
        json_list["arguments"].push_back(argument);
    }

    template<typename Argument,typename...Arguments>
    void to_json_list(Argument argument,Arguments...arguments){
        argument_to_json(argument);
        if constexpr(sizeof...(arguments) > 0){
            to_json_list(arguments...);
        }
    }

    bool handle_situation();
    // template<typename Result>
    // bool type_judge(){
    //     if constexpr(std::is_same_v<Result,int> || std::is_same_v<Result,double> || std::is_same_v<Result,float> || std::is_same_v<Result,short>){
    //         if(json_list["data"].is_number()){
    //             return true;
    //         }
    //     }
    //     else if constexpr(std::is_same_v<Result,std::vector<int>> || std::is_same_v<Result,std::vector<char>> ||)
    // }

public:
    Rpc(std::string ip,short port);
    ~Rpc();
    std::string get_error_message(){
        return error_message;
    }
    template<typename Result,typename...Arguments>
    Result call(std::string funcname,Arguments...argements){
        funcname_to_json(funcname);
        json_list["arguments"] = json::array();
        to_json_list(argements...);
        //std::cout<<json_list<<std::endl;
        std::string data_string = json_list.dump();
        std::cerr << data_string << " %%\n";
        std::string s = ip.append(":").append(std::to_string(port).append("/funcname"));
        http_headers headers;
        headers["content-type"] = "application/json";
        auto resp = requests::post(s.c_str(),data_string, headers);
        json_list.clear();
        json_list = json::parse(resp->body); 
        // if(!handle_situation()){
        //     return {};
        // }
        auto res = json_list["data"].get<Result>();
        return res;
    };
};