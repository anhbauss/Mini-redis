
#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

std::string readInput();
std::vector<std::string> tokenize(const std::string& input);
std::string encodeRESPArray(const std::vector<std::string>& tokens);
int connectToServer(const std::string& host, int port);
bool sendRequest(int arduinoBK_socket, const std::string& request);
std::string readResponse(int arduinoBK_socket);
std::string parseResponse(const std::string& resp);
void displayResult(const std::string& result);

#endif // CLIENT_H

