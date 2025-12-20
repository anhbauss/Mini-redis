
#ifndef ISERVER_COMMANDS_H
#define ISERVER_COMMANDS_H

#include <string>
#include <vector>
#include <unordered_map>

std::string handleSET(const std::vector<std::string>& args, 
                     std::unordered_map<std::string, std::string>& store);
std::string handleGET(const std::vector<std::string>& args, 
                     std::unordered_map<std::string, std::string>& store);
int handleDEL(const std::vector<std::string>& args, 
              std::unordered_map<std::string, std::string>& store);
int handleEXISTS(const std::vector<std::string>& args, 
                 std::unordered_map<std::string, std::string>& store);

#endif // SERVER_COMMANDS_H