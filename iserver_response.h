#ifndef ISERVER_RESPONSE_H
#define ISERVER_RESPONSE_H

#include <string>

bool sendResponse(int client, const std::string& response);
std::string encodeCommandResult(const std::string& process_result);

#endif // SERVER_RESPONSE_H
