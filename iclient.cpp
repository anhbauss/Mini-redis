#include "client.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "RESP_ALGORITHM.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//input handle
std::string readInput(){
    std::string line;
    cout << "mini-redis";
    getline(cin, line);
    return line;
}
//tokenize lại request
std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    size_t start = input.find_first_not_of(" "); // Tìm chữ cái đầu tiên không phải khoảng trắng
    
    while (start != std::string::npos) {
        size_t end = input.find_first_of(" ", start); // Tìm khoảng trắng tiếp theo
        
        // Cắt từ vị trí start với độ dài (end - start)
        tokens.push_back(input.substr(start, end - start));
        
        // Tìm chữ cái tiếp theo (bỏ qua các khoảng trắng ở giữa)
        start = input.find_first_not_of(" ", end);
    }
    
    return tokens;
}
//encode RESP
std::string encodeRESPArray(const std::vector<std::string>&tokens)
{
    std::string data = pack_array(tokens);
    return data;
}
int connectToServer(const std::string& host, int port)
{
    int arduinoBK_socket = socket(AF_INET, SOCK_STREAM,0);
    if (arduinoBK_socket == INVALID_SOCKET)
    {
      perror("Create socket failed with error");
      return -1;    
    }
    // Thiết lập kết nối tới client
    struct sockaddr_in clientConnect;
    clientConnect.sin_family = AF_INET;                     // Sử dụng giao thức IPv4
    clientConnect.sin_port = htons(port);                   // Cổng mà server sẽ lắng nghe
     inet_pton(AF_INET, host.c_str(),&clientConnect.sin_addr);
    int connectresult;
    connectresult = connect(arduinoBK_socket, (struct sockaddr *)&clientConnect, sizeof(clientConnect));
    if (connectresult == SOCKET_ERROR)
    {
        perror ("Connect to client failed with error ");
        closesocket(arduinoBK_socket); // Đóng socket
        return -1;
    }
    else
    {
        return arduinoBK_socket;
    }

}
bool sendRequest(int arduinoBK_socket, const std::string& request) {
    int bytes_sent = send(arduinoBK_socket, request.c_str(),request.length(),0);
    if bytes_sent == request.length() return true;
    else return false;
}
string readLine(int sock) {
    std::string line;
    char c;
    while (recv(sock, &c, 1, 0) > 0) {
        if (c == '\n') {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            break;
        }
        line += c;
    }
    return line;
}
std::string readResponse(int arduinoBK_socket) {
    std::string response;
    char first_char;
    
    response += first_char;

    if (first_char == '+' || first_char == '-' || first_char == ':') {
        // 1. Lấy dữ liệu sạch
        std::string data = readLine(arduinoBK_socket);
        // 2. Thêm vào response kèm theo dấu ngắt dòng tiêu chuẩn
        response += data + "\r\n";
    } 
    else if (first_char == '$') {
        // Đọc dòng chứa độ dài
        std::string len_str = readLine(arduinoBK_socket);
        int len = std::stoi(len_str);
        
        // Gắn lại dòng độ dài vào response
        response += len_str + "\r\n";

        if (len == -1) return response; // Null Bulk String ($-1\r\n)

        // Đọc phần nội dung (Bulk Data)
        // Chúng ta cần đọc đúng 'len' bytes + 2 bytes của CRLF kết thúc
        int total_to_read = len + 2;
        std::string buffer(total_to_read, '\0');
        int bytes_received = 0;

        while (bytes_received < total_to_read) {
            int r = recv(arduinoBK_socket, &buffer[bytes_received], total_to_read - bytes_received, 0);
            if (r <= 0) break;
            bytes_received += r;
        }
        
        // Thêm toàn bộ phần buffer (đã bao gồm \r\n ở cuối) vào response
        response += buffer;
    }
    
    return response;
}
std::string parseResponse(const std::string& resp){
    if (resp.empty()) return "";
    char type = resp[0];
    switch (type)
    case '+': return unpack_simple_string(resp);
    case '-': return unpack_error(resp);
    case ':': return unpack_integer(resp);
    case '$': return unpack_bulk_string(resp);
    default return "Unknown response type";
}
void displayResult(const std::string& result) {
    std::cout << result << std::endl;
}
int main() {
    int arduinoBK_socket = connectToServer("localhost", 6379);
    if (arduinoBK_socket < 0) {
        std::cout << "Could not connect to server" << endl;
        return 1;
    }
    
    while (true) {
        // 1. Đọc input
        std::string input = readInput();
        
        // 2. Xử lý lệnh đặc biệt
        if (input == "QUIT" || input == "exit") {
            break;
        }
        if (input.empty()) {
            continue;
        }
        
        // 3. Tokenize
        std::vector<std::string> tokens = tokenize(input);
        if (tokens.empty()) {
            continue;
        }
        
        // 4. Encode RESP
        std::string request = encodeRESPArray(tokens);
        
        // 5. Gửi request
        sendRequest(arduinoBK_socket, request);
        
        // 6. Nhận response
        std::string response = readResponse(sock);
        
        // 7. Parse và hiển thị
        std::string result = parseResponse(response);
        displayResult(result);
    }
    
    close(arduinoBK_socket);
    return 0;
}