#include "server.h"
#include <hv/HttpServer.h>
#include <unordered_map>


std::any sum(const std::vector<std::any> &arguments){
    int argument_first = std::any_cast<int> (arguments[0]);
    int argument_second = std::any_cast<int> (arguments[1]);
    return argument_first + argument_second;
}
std::any sub(const std::vector<std::any> &arguments){
    int argument_first = std::any_cast<int> (arguments[0]);
    int argument_second = std::any_cast<int> (arguments[1]);
    return argument_first - argument_second;
} 
std::any appends(const std::vector<std::any> &arguments){
    std::string argument_first = std::any_cast<std::string> (arguments[0]);
    std::string argument_second = std::any_cast<std::string> (arguments[1]);
    return argument_first.append(argument_second);
}
std::unordered_map<std::string,FuncObject*> initial_funcobject(){
    //std::vector<FuncObject*> func_list;
    std::unordered_map<std::string,FuncObject*> func_map;
    FuncObject *func1 = new FuncObject;
    func1->funcname = "sum";
    func1->arguments_num = 2;
    func1->type_arguments.push_back("int");
    func1->type_arguments.push_back("int");
    func1->type_return = "int";
    func1->func = sum;
    func_map[func1->funcname] = func1;
    //func_list.push_back(func1);
    FuncObject *func2 = new FuncObject;
    func2->funcname = "sub";
    func2->arguments_num = 2;
    func2->type_arguments.push_back("int");
    func2->type_arguments.push_back("int");
    func2->type_return = "int";
    func2->func = sub;
    func_map[func2->funcname] = func2;
    //func_list.push_back(func2);
    FuncObject *func3 = new FuncObject;
    func3->funcname = "appends";
    func3->arguments_num = 2;
    func3->type_arguments.push_back("string");
    func3->type_arguments.push_back("string");
    func3->type_return = "string";
    func3->func = appends;
    func_map[func3->funcname] = func3;
    return func_map;
    //func_list.push_back(func3);
}
void operation(json &json_return, const FuncObject* target_json,const std::vector<std::any> &arguments_array){
    auto temp_used = target_json->func(arguments_array);
    if(target_json->type_return == "int"){
        int result = std::any_cast<int>(temp_used);
        json_return["data"] = result;
    } else if(target_json->type_return == "string"){
        std::string result = std::any_cast<std::string>(temp_used);
        json_return["data"] = result;
    } else if(target_json->type_return == "double"){
        double result = std::any_cast<double>(temp_used);
        json_return["data"] = result;
    } else if(target_json->type_return == "float"){
        float result = std::any_cast<float>(temp_used);
        json_return["data"] = result;
    } else if(target_json->type_return == "vector<int>"){
        std::vector<int> result = std::any_cast<std::vector<int>>(temp_used);
        json_return["data"] = result;
    } else if(target_json->type_return == "vector<string>"){
        std::vector<std::string> result = std::any_cast<std::vector<std::string>>(temp_used);
        json_return["data"] = result;
    }
}
bool type_compare(const json &argument,const std::string &type,std::any &value){
    if(argument.is_array()){
        if(argument[0].is_boolean() && type == "bool"){
            std::vector<bool> argument_list_bool;
            for(const auto &temp: argument){
                if(!temp.is_boolean()){
                    return false;
                }
                argument_list_bool.push_back(temp.get<bool>());
            }
            value = argument_list_bool;
            return true;
        } else if(argument[0].is_number_float()){
            if(type == "float"){
                std::vector<float> argument_list_float;
                for(const auto &temp: argument){
                    if(!temp.is_number_float()){
                        return false;
                    }
                    argument_list_float.push_back(temp.get<float>());
                }
                value = argument_list_float;
                return true;
            } else if(type == "double"){
                std::vector<double> argument_list_double;
                for(const auto &temp: argument){
                    if(!temp.is_number_float()){
                        return false;
                    }
                    argument_list_double.push_back(temp.get<double>());
                }
                value = argument_list_double;
                return true;
            }
        } else if(argument[0].is_number_integer() && type == "int"){
            std::vector<int> argument_list_int;
            for(const auto &temp: argument){
                if(!temp.is_number_integer()){
                        return false;
                    }
                argument_list_int.push_back(temp.get<int>());
            }
            value = argument_list_int;
            return true;
        } else if(argument[0].is_string() && type == "string"){
            std::vector<std::string> argument_list_string;
            for(const auto &temp: argument){
                if(!temp.is_string()){
                        return false;
                    }
                argument_list_string.push_back(temp.get<std::string>());
            }
            value = argument_list_string;
            return true;
    }
    } else if(argument.is_boolean() && type == "bool"){
        value = argument.get<bool>();
        return true;
    } else if(argument.is_number_float()){
        if(type == "float"){
                value = argument.get<float>();
                return true;
            } else if(type == "double"){
                value = argument.get<double>();
                return true;
            }
    } else if(argument.is_number_integer() && type == "int"){
        value = argument.get<int>();
        return true;
    } else if(argument.is_string() && type == "string"){
        value = argument.get<std::string>();
        return true;
    }
    return false;
}
json check_aeguments(const json &json_list,FuncObject* target_json){
    json json_return = json::object();
    if(json_list["arguments"].size() != target_json->arguments_num){
        json_return["code"] = status::ARGUMENT_NUM_ERROR;
        json_return["message"] = "The aeguments number needed is " + std::to_string(target_json->arguments_num);
        return json_return;
    }
    std::vector<std::any> arguments_array;
    for(int i = 0; i < target_json->arguments_num; i++){
        std::any value;
        if(!type_compare(json_list["arguments"][i],target_json->type_arguments[i],value)){
            json_return["code"] = status::ARGUMENT_INVALID;
            json_return["message"] = "The type of aegument is invalid";
            return json_return;
        }
        arguments_array.push_back(value);
    }
    operation(json_return,target_json,arguments_array);
    json_return["code"] = status::SUCCESSS;
    // std::vector<std::any> t;
    // t.push_back(json_list["arguments"][0].get<int>());
    // target_json->func(t);
    return json_return;
}
json try_call(const json &json_list, const std::unordered_map<std::string, FuncObject*> &func_map){
    json json_return = json::object();
    std::string funcname_temp = json_list["funcname"].get<std::string>();
    //FuncObject temp_used;
    if(func_map.find(funcname_temp) != func_map.end()){
        json_return = check_aeguments(json_list,func_map.find(funcname_temp)->second);
    } else {
        json_return["code"] = status::FUNCNAME_NOTFOUND;
    }
    return json_return;
}

int main(){
    HttpService router;
    auto func_map = initial_funcobject();
    router.POST("/funcname", [func_map](const HttpContextPtr& ctx) {
        json json_list = ctx->json();
        json json_return = try_call(json_list, func_map);
        return ctx->sendJson(json_return);
    });
    http_server_t server;
    server.port = 8080;
    server.service = &router;
    http_server_run(&server);
    return 0;
}