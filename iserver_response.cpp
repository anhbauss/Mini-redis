#include "iserver_response.h"
#include "iserver_tcp.h"
#include "iserver_parse.h"
#include "iserver_commands.h"
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include "RESP_ALGORITHM.h"
bool sendResponse(int client, const std::string&response){
    size_t total_sent = 0;
    size_t total_len = response.length();
    const char* data = response.c_str();
    // Gửi cho đến khi gửi hết dữ liệu
    while (total_sent < total_len) {
        int bytes_sent = send(client_fd, 
                              data + total_sent, 
                              total_len - total_sent, 
                              0);
        
        if (bytes_sent < 0) {
            perror("send failed");
            return false;
        }
        
        if (bytes_sent == 0) {
            // Client đóng kết nối
            return false;
        }
        
        total_sent += bytes_sent;
    }
    
    return true;
}
std::string encodeCommandResult(const std::string& process_result){
    if (process_result.find("OK:"==0))
    {
        return pack_simple_string("OK");
    }
    else if (process_result.find("ERROR:")==0){
        std::string error_msg = process_result.substr(6); // Bỏ qua "ERROR:"
        return pack_error(error_msg);
    }
    else if (process_result.find("VALUE:")==0){
         std::string value = process_result.substr(6);
         return pack_bulk_string(value);
    }
    else if (process_result == "NULL") {
        return pack_bulk_string();
    } else if (process_result.find("INT:") == 0) {
        int num = std::stoi(process_result.substr(4)); // Bỏ qua "INT:"
        return pack_integer(num);
    }
    return pack_error("unknown result format");
}