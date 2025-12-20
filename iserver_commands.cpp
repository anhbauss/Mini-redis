#include "server_commands.h"
#include <unordered_map>
#include <vector>
#include <string>
//Xử lý command
std::string handleSET(const std::vector<std::string>&args,
    std::unordered_map<std::string, std::string>& store)
{
    std::string key= args[1];
    std::string value= args[2];
    store[key]=value;
    return "OK";
}
std::string handleGET(const std::vector<std::string>& args, 
                     std::unordered_map<std::string, std::string>& store) {
    std::string key = args[1];
    auto it = store.find(key);
    if (it != store.end()){
        return it->second;
    }
    return "";
}
int handleDEL(const std::vector<std::string>& args, 
              std::unordered_map<std::string, std::string>& store) {
    std::string key = args[1];
    return (int)store.erase(key);
}
int handleEXISTS(const std::vector<std::string>& args, 
                 std::unordered_map<std::string, std::string>& store) {
        std::string key = args[1];
        return (int)store.count(key);
}

