#include "iserver_tcp.h"
#include "iserver_parse.h"
#include "iserver_commands.h"
#include "iserver_response.h"
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include "RESP_ALGORITHM.h"
int main() {
    // Khởi tạo hash table
    std::unordered_map<std::string, std::string> store;
    
    // Khởi tạo server
    int arduinoBK_socket = initializeServer(6379);
    if (arduinoBK_socket < 0) {
        return 1;
    }
    
    // Vòng lặp chính
    while (true) {
        // 1. Chấp nhận client
        int client = acceptClient(arduinoBK_socket);
        if (client < 0) {
            continue; // Thử lại
        }
        
        // 2. Xử lý request từ client (có thể nhiều request)
        bool client_connected = true;
        while (client_connected) {
            // 2a. Nhận request
            std::string request = receiveRequest(client);
            if (request.empty()) {
                // Client đóng kết nối
                client_connected = false;
                break;
            }
            
            // 2b. Parse RESP
            std::vector<std::string> args;
            try {
                args = unpack_array(request);
            } catch (const char* error) {
                std::string error_response = pack_error("protocol error: " + std::string(error));
                sendResponse(client, error_response);
                continue;
            }
            
            // 2c. Validate và process
            std::string response = process_command(args, store);
            
            // 2d. Gửi response
            if (!sendResponse(client, response)) {
                client_connected = false;
                break;
            }
        }
        
        // 3. Đóng kết nối client
        close(client);
        std::cout<<"Client disconnected"<<std::endl;
    }
    
    // Cleanup (không bao giờ đến đây trong vòng lặp vô hạn)
    close(arduinoBK_socket);
    return 0;
}