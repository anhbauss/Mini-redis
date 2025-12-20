#ifndef ISERVER_TCP_H
#define ISERVER_TCP_H

#include <string>

int initializeServer(int port);
int acceptClient(int server_fd);
std::string receiveRequest(int client_fd);

#endif // SERVER_TCP_H

