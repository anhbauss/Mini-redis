#ifndef ISERVER_PARSE_H
#define ISERVER_PARSE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include "../../include/interfaces.h"
struct CommandCheck {
    bool valid;
    std::string command;
    std::string error_message;
};
std::vector<std::string> parseRESPArray(const std::string& resp);
CommandCheck validate_Command(const std::vector<std::string>& args);
std::string process_command(const std::vector<std::string>& args, 
                            std::unordered_map<std::string, std::string>& store);

#endif // SERVER_PARSE_H

