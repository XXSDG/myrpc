#include <hv/HttpServer.h>
#include <string>
#include <any>
#include <vector>
#include <hv/json.hpp>
using json = nlohmann::json;
struct FuncObject{
    std::string funcname;
    int arguments_num;
    std::vector<std::string> type_arguments;
    std::string  type_return;
    std::function<std::any(const std::vector<std::any>&)> func;
};
enum class status:int{
    SUCCESSS,
    FUNCNAME_NOTFOUND,
    ARGUMENT_INVALID,
    ARGUMENT_NUM_ERROR
};
std::unordered_map<std::string, FuncObject*> initial_funcobject();
std::any sum(const std::vector<std::any> &arguments);
std::any sub(const std::vector<std::any> &arguments);
std::any appends(const std::vector<std::any> &arguments);
json try_call(const json &json_list,const std::unordered_map<std::string, FuncObject*> &func_list);
json check_aeguments(const json &json_list,const FuncObject* target_json);
bool type_compare(const json &argument,const std::string &type,std::any &value);
void operation(json &json_return, const FuncObject* target_json,const std::vector<std::any> &arguments_array);