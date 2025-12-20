#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <unordered_map>
#include <string>
int intializeServer(int port)
{
    // Tạo một Socket object
    int arduinoBK_socket;
    arduinoBK_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Dùng giao thức IPv4, TCP, Stream socket
    if (arduinoBK_socket == INVALID_SOCKET)
    {
        perror ("Creat socket failed with error: ");                                                                   // Giải phóng tài nguyên Winsock
        return -1;
    }
    else
    {
        std::cout << "Socket created successfully." << std::endl;
    }
    //Cấu hình socket options
    int opt =1;
    setsockopt(arduinoBK_socket, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
    // bind một socket
    // int bind(
    //  SOCKET s,                    // Socket để gán địa chỉ
    //  const struct sockaddr *name, // Con trỏ tới cấu trúc sockaddr chứa địa chỉ
    //  int namelen                  // Kích thước của cấu trúc sockaddr
    //);
    // tạo và thiết lập giá trị cho một object kiểu sockaddr_in
    struct sockaddr_in arduinoBKsvr_addr;
    arduinoBKsvr_addr.sin_family = AF_INET;                     // Sử dụng giao thức IPv4
    arduinoBKsvr_addr.sin_addr.s_addr = INADDR_ANY; // Địa chỉ IP của server bất kỳ nào
    arduinoBKsvr_addr.sin_port = htons(port);                   // Cổng mà server sẽ lắng nghe
    // Gán địa chỉ cho socket
    int bindResult;
    bindResult = bind(arduinoBK_socket, (struct sockaddr *)&arduinoBKsvr_addr, sizeof(arduinoBKsvr_addr));
    if (bindResult == SOCKET_ERROR)
    {
        perror ("Bind socket failed with error: ");
        close(arduinoBK_socket);
        return -1;
    }
    // hàm listen
    // int listen(
    //     SOCKET s, // Socket để lắng nghe kết nối
    //     int backlog // Số lượng kết nối tối đa trong hàng đợi
    //);
    //  Lắng nghe kết nối từ client
    if (listen(arduinoBK_socket, 1) == SOCKET_ERROR)
    {
        perror ("Listening connection failed with error: ");
        close(arduinoBK_socket);
        return -1;
    }
    else
    {
        std::cout << "Server is listening in port"<<port<< std::endl;
        return arduinoBK_socket;
    }
}
    // Hàm accept
    // SOCKET accept(
    //   SOCKET s,               // Socket đang lắng nghe kết nối
    // struct sockaddr * addr, // Con trỏ tới cấu trúc sockaddr để lưu thông tin client
    //  int *addrlen            // Con trỏ tới kích thước của cấu trúc sockaddr
    //  );
    // Tạo một socket tạm thời để chấp nhận kết nối từ client
int acceptClient(int arduinoBK_socket){    
    struct sockaddr_in client_address;
    socklen_t client_addr_len = sizeof(client_address);
    std::cout << "Waiting for connection from client..." << std::endl;
    int client = accept(arduinoBK_socket, (struct sockaddr *)&client_address,&client_addr_len);
    if (client < 0){
        std::cerr <<"accept failed"<<std::endl;
        return -1;
    }    
    char client_ip[INET_ADDRSTRLEN];//Tạo buffer chứa chuỗi IP
    inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);
    std::cout << "Client IP: "<< client_ip << std::endl;
    std::cout <<"Port: "<<ntohs(client_address.sin_port)<<std::endl;
    return client;
}
std::string receiveRequest(int client){
    std::string full_request = "";
    char buffer[4096]= {0};
    int bytes_read = recv(client, buffer, sizeof(buffer)-1,0);
    if (bytes_read < 0) {
        perror("recv failed");
        return "";
    }
    if (bytes_read == 0) {
        // Client đóng kết nối
        return "";
    }
    buffer[bytes_read]='\0';
    std::string raw_resp(buffer);
    return raw_resp;
}

std::string receiveFullRequest(int client) {
    std::string full_request = "";
    char buffer[1024];
    
    while (true) {
        int bytes_read = recv(client, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_read <= 0) {
            break;
        }
        
        buffer[bytes_read] = '\0';
        full_request += std::string(buffer, bytes_read);
        
        // Kiểm tra xem đã đủ RESP message chưa
        // (Có thể parse thử để kiểm tra)
        if (isCompleteRESPMessage(full_request)) {
            break;
        }
    }
    
    return full_request;
}
std::unordered_map<std::string,std::string> store;