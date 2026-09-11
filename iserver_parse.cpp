#include "iserver_commands.h"
#include "../../include/interfaces.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <variant>
#include "RESP_ALGORITHM.h"
using namespace std;
//parse code ra
vector<string> parseRESPArray(const string& resp){
  vector<string> result = unpack_array(resp);  
return result;
}
string toUpperCase(const string& str){
    string result = str;
    transform(result.begin(),result.end(),result.begin(),::toupper);
    return result;
}
struct CommandCheck {
    bool valid;
    string command;
    string error_message;
};
//kiểm tra cú pháp
CommandCheck validate_Command(const vector<string>& args){
    CommandCheck result;
    if(args.empty()){
        result.valid= false;
        result.error_message = "empty command";
        return result;
    }
    result.command = toUpperCase(args[0]);
    vector<string> valid_commands = {"SET", "GET", "DEL", "EXISTS"};
    bool is_valid_command = false;
    //Check xem đúng với command nào không
    for (const string& cmd : valid_commands ){
        if (result.command == cmd){
            is_valid_command = true;
            break;
        }
    }
    if (is_valid_command==false){
        result.valid = false;
        result.error_message = "Deo bt command '" + args[0] + "'" + "la clg";
        return result; 
    }
    //Kiểm tra số lượng args
    int num_args = 0;
    if (result.command == "SET") {
        num_args = 3;
    } else if (result.command == "GET" || 
               result.command == "DEL" || 
               result.command == "EXISTS") {
        num_args = 2;
    }
    if (args.size() != num_args) {
        result.valid = false;
        result.error_message = "wrong number of arguments for '" + 
                              result.command + "' command";
        return result;
    }
    result.valid = true;
    return result;
}
//Xử lý cú pháp
string process_command(const vector<string>& args, unordered_map<string, string>& store)
{
CommandCheck validation = validate_Command(args);
if (validation.valid=false){
    return "ERROR:"+ validation.error_message;
}
string command = validation.command;
if (command == "SET"){
    return "OK:" + handleSET(args, store);
}
else if (command == "GET") {
    string result = handleGET(args, store);
    if (result.empty()==true){
        return "NULL";
    }    
return "VALUE:" + result;
} 
else if (command == "DEL") {
        int result =  handleDEL(args, store);
        return "INT:" + to_string(result);
    } 
else if (command == "EXISTS") {
         int result = handleEXISTS(args, store);
         return "INT:" + to_string(result);
    }
    return "ERROR:unknown command";
}
