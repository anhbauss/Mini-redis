#ifndef RESP_ALGORITHM_H
#define RESP_ALGORITHM_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <cstring>

// Pack kiểu Bulk String
std::string pack_bulk_string(const std::string &data);
// Unpack kiểu Bulk String
std::string unpack_bulk_string(const std::string &resp);

// Pack kiểu Simple String
std::string pack_simple_string(const std::string &data);
// Unpack kiểu Simple String
std::string unpack_simple_string(const std::string &resp);

// Pack kiểu Integer
std::string pack_integer(int data);
// Unpack kiểu Integer
int unpack_integer(const std::string &resp);

// Pack kiểu Error
std::string pack_error(const std::string &data);
// Unpack kiểu Error
std::string unpack_error(const std::string &resp);
// Pack Booleans
std::string pack_boolean(bool data);
// Unpack Booleans
bool unpack_boolean(const std::string &resp);

// Pack kiểu Double
std::string pack_double(double data);
// Unpack kiểu Double
double unpack_double(const std::string &resp);

// Pack kiểu Big number
std::string pack_bignumber(const std::string &data);
// Unpack kiểu Big number
std::string unpack_bignumber(const std::string &resp);

// Pack Null
std::string pack_null();
// Unpack Null
bool unpack_null(const std::string &resp);

// Pack Bulk Error
std::string pack_bulk_error(const std::string &data);
// Unpack Bulk Error
std::string unpack_bulk_error(const std::string &resp);

// Pack Verbatim String
std::string pack_verbatim_string(std::string encoding, const std::string &data);
// Unpack Verbatim String
std::string unpack_verbatim_string(const std::string &resp);
// Pack Array String
std::string pack_array(const std::vector<std::string> &command_parts);
// Unpack Arrray String
std::vector<std::string> unpack_array(const std::string &resp);
// Pack Map String
std::string pack_map(std::vector<std::pair<std::string, std::string>> &data);
// Unpack Map String
std::vector<std::pair<std::string, std::string>> unpack_map(std::string resp);
#endif
