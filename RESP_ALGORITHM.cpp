// Tạo thư viển để pack và phân giải Resp
// Chứa các hàm sử dụng dữ liệu con (bulkstring, simplestring, interger. double, boolean,...)
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
// Ham find num_rn
size_t find_num_rn(std::string resp, int pointer)
{
    size_t num_rn = resp.find("\r\n", pointer);
    return num_rn;
}
// Pack kiểu Bulk String
std::string pack_bulk_string(const std::string &data)
{
    int len = data.length();
    std::string len_str = std::to_string(len);
    std::string result = "$" + len_str + "\r\n" + data + "\r\n";
    return result;
}
// Unpack kiểu Bulk String
std::string unpack_bulk_string(const std::string &resp)
{
    if (resp[0] != '$' || resp.empty() == true)
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
        return "";
    }
    size_t num_rn = resp.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu \r\n
    if (num_rn == std::string::npos)      // Neu khong tim thay \r\n, npos là giá trị đặc biệt báo lỗi
    {
        std::cerr << "Loi dinh dang phan giai(No \\r\\n on bulk string)" << std::endl;
        return "";
    }
    std::string len_str = resp.substr(1, num_rn - 1); // Lấy độ dài chuỗi kiểu string
    long long len = std::stoi(len_str);               // Chuyển đổi độ dài chuỗi sang kiểu int
    try
    {
        len = std::stoll(len_str); // Chuyển đổi độ dài chuỗi sang kiểu long long
    }
    catch (const std::exception &e)
    {
        std::cerr << "Loi dinh dang phan giai(Khong the chuyen doi do dai chuoi)" << e.what() << std::endl;
        return "";
    }
    // Xử lý null bulk string ($-1\r\n)
    if (len == -1)
    {
        return ""; // Trả về chuỗi rỗng
    }
    // Lấy phần dữ liệu chuỗi
    size_t nextpos = num_rn + 2;
    if (nextpos + len + 2 > resp.length())
    {
        std::cerr << "Loi dinh dang phan giai(Khong du du lieu de phan giai)" << std::endl;
        return "";
    }
    std::string data = resp.substr(nextpos, len); // Lấy dữ liệu chuỗi
    // Kiem tra \r\n cuoi cung
    if (resp.substr(nextpos + len, 2) != "\r\n")
    {
        std::cerr << "Loi dinh dang phan giai(Khong tim thay \\r\\n cuoi cung)" << std::endl;
        return "";
    }
    return data; // Trả về dữ liệu chuỗi
}
// Pack kiểu Simple String
std::string pack_simple_string(const std::string &data)
{
    std::string result = "+" + data + "\r\n";
    return result;
}
// Unpack kiểu Simple String
std::string unpack_simple_string(const std::string &resp)
{
    if (resp[0] != '+')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
    }
    size_t num_rn = resp.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu +
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
    }
    std::string data = resp.substr(1, num_rn - 1); // Lấy dữ liệu chuỗi
    return data;                                   // Trả về dữ liệu chuỗi
}
// Pack kiểu Integer
std::string pack_integer(int data)
{
    std::string data_str = std::to_string(data);
    std::string result = ":" + data_str + "\r\n";
    return result;
}
// Unpack kiểu Integer
int unpack_integer(const std::string &resp)
{
    if (resp[0] != ':')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
    }
    size_t num_rn = resp.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu :
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
    }
    else
    {
        // tiếp tục xử lý
        std::string data_str = resp.substr(1, num_rn - 1); // Lấy dữ liệu chuỗi
        int data = std::stoi(data_str);                    // Chuyển đổi chuỗi sang kiểu int
        return data;                                       // Trả về dữ liệu kiểu int
    }
    return 0; // Trả về 0 nếu có lỗi
}
// Pack kiểu Error
std::string pack_error(const std::string &data)
{
    std::string result = "-" + data + "\r\n";
    return result;
}
// Unpack kiểu Error
std::string unpack_error(const std::string &resp)
{
    if (resp[0] != '-')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
    }
    size_t num_rn = resp.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu -
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
    }
    std::string data = resp.substr(1, num_rn - 1); // Lấy dữ liệu chuỗi
    return data;                                   // Trả về dữ liệu chuỗi
}
// pack booleans
std::string pack_boolean(bool data)
{
    if (data == true)
    {
        return std::string("#t\r\n");
    }
    else
    {
        return std::string("#f\r\n");
    }
}
// unpack booleans
bool unpack_boolean(const std::string &resp)
{
    if (resp.find("#t\r\n") == 0)
    {
        return true;
    }
    else if (resp.find("#f\r\n") == 0)
    {
        return false;
    }
    else
    {
        std::cerr << "Loi dinh dang phan giai boolean" << std::endl;
        return false;
    }
}
// pack kiểu dữ liệu double
std::string pack_double(double data)
{
    int int_part = static_cast<int>(data);
    std::string int_part_str = std::to_string(int_part);
    double frac_part = data - int_part;
    std::string frac_part_str = std::to_string(frac_part).substr(2); // lấy phần sau dấu chấm
    std::string result = "," + int_part_str + "." + frac_part_str + "\r\n";
    return result;
}
// unpack kiểu dữ liệu double
double unpack_double(const std::string &resp)
{
    if (resp[0] != ',')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
        return 0.0;
    }
    size_t num_rn = resp.find("\r\n", 1); // tim vi tri cua \r\n dau tien
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
        return 0.0;
    }
    std::string data_str = resp.substr(1, num_rn - 1); // Lay du lieu chuoi
    double data;
    try
    {
        data = std::stod(data_str); // Chuyen doi chuoi sang kieu double
    }
    catch (const std::exception &e)
    {
        std::cerr << "Loi dinh dang phan giai(Khong the chuyen doi sang double)" << e.what() << std::endl;
        return 0.0;
    }
    return data; // Tra ve du lieu kieu double
    return 0;
}
// pack dữ liệu Big number (sử dụng kiểu string để biểu diễn)
std::string pack_bignumber(const std::string &data)
{
    std::string result = "(" + data + "\r\n";
    return result;
}
// unpack dữ liệu Big number
std::string unpack_bignumber(const std::string &resp)
{
    if (resp[0] != '(')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
    }
    size_t num_rn = resp.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu (
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
    }
    std::string data = resp.substr(1, num_rn - 1); // Lấy dữ liệu chuỗi
    return data;                                   // Trả về dữ liệu chuỗi
}
// pack Null
std::string pack_null()
{
    return std::string("_\r\n");
}
bool unpack_null(const std::string &resp)
{
    if (resp == "_\r\n")
    {
        return true; // là null
    }
    else
    {
        return false; // không phải null
    }
}
// pack bulk error
std::string pack_bulk_error(const std::string &data)
{
    int len = std::string(data).length();
    std::string len_str = std::to_string(len);
    std::string result = "!" + len_str + "\r\n" + "data " + "\r\n";
    return result;
}
// unpack bulk error
std::string unpack_bulk_error(const std::string &resp)
{
    if (resp[0] != '!')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
        return "";
    }
    size_t num_rn = resp.find("\r\n", 1);
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai(No \\r\\n on bulk error)" << std::endl;
        return "";
    }
    std::string len_str = resp.substr(1, num_rn - 1);
    long long len;
    try
    {
        len = std::stoll(len_str);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Khong the chuyen do dai chuoi" << e.what() << std::endl;
        return "";
    }
    size_t nextpos = num_rn + 2;
    if (nextpos + len + 2 > resp.length())
    {
        std::cerr << "Chuoi khong co du lieu de phan giai" << std::endl;
        return "";
    }
    std::string data = resp.substr(nextpos, len);
    if (resp.substr(nextpos + len, 2) != "\r\n")
    {
        std::cerr << "Khong tim thay \\r\\n cuoi cung" << std::endl;
        return "";
    }
    return data; // Trả về dữ liệu chuỗi
}
// Pack Verbatim String
std::string pack_verbatim_string(std::string encoding, const std::string &data)
{
    size_t len = data.length() + encoding.length() + 1; // +1 để tính dấu :
    std::string len_str = std::to_string(len);
    std::string result = "=" + len_str + "\r\n" + encoding + ":" + "data" + "\r\n";
    return result;
}
// Unpack Verbatim String
std::string unpack_verbatim_string(const std::string &resp)
{
    if (resp[0] != '=' || resp.empty() == true)
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
        return "";
    }
    size_t num_rn = resp.find("\r\n", 1);
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai(No \\r\\n on vervatim str" << std::endl;
        return "";
    }
    std::string len_str = resp.substr(1, num_rn - 1);
    long long len;
    try
    {
        len = std::stoll(len_str);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Loi dinh dang phan giai(Khong the chuyen do dai chuoi)" << e.what() << std::endl;
        return "";
    }
    size_t nextpos = num_rn + 2;
    if (nextpos + len + 2 > resp.length())
    {
        std::cerr << "Loi dinh dang phan giai(Khong du du lieu de phan giai)" << std::endl;
        return "";
    }
    std::string data = resp.substr(nextpos, len);
    if (resp.substr(nextpos + len, 2) != "\r\n")
    {
        std::cerr << "Loi dinh dang phan giai(Khong tim thay \\r\\n cuoi cung)" << std::endl;
        return "";
    }
    return data; // Trả về dữ liệu chuỗi
}
// Pack kiểu Arrays
std::string pack_array(const std::vector<std::string> &command_parts)
{
    size_t num_elements = command_parts.size();
    std::string result = "*" + std::to_string(num_elements) + "\r\n";
    for (const auto &part : command_parts) // Vòng lặp foreach duyệt qua các phần tử trong command_parts
    {
        result += pack_bulk_string(part);
    }
    return result;
}
// Unpack kiểu Arrays
std::vector<std::string> unpack_array(const std::string &resp)
{
    // copy resp de thao tac
    std::string resp_copy = resp;
    if (resp_copy[0] != '*' || resp_copy.empty() == true)
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
        return {};
    }
    size_t num_rn = resp_copy.find("\r\n", 1); // Tim vi tri cua \r\n dau tien sau ky tu *
    if (num_rn == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
        return {};
    }
    std::string num_elements_str = resp_copy.substr(1, num_rn - 1); // Lấy số phần tử kiểu string
    long long num_elements;
    try
    {
        num_elements = std::stoll(num_elements_str);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Loi dinh dang phan giai(Khong the chuyen doi so phan tu)" << e.what() << std::endl;
        return {};
    }
    resp_copy.erase(0, num_rn + 2); // Xóa phần đầu của chuỗi đến sau \r\n
    // Gọi hàm unpack_bulk_string nhiều lần để lấy từng phần tử trong mảng
    std::vector<std::string> outputarrray;
    for (int i = 0; i < num_elements; i++)
    {
        std::string element = unpack_bulk_string(resp_copy);
        if (element.empty() == true && resp_copy.substr(0, 4) == "$-1\r\n")
        {
            resp_copy.erase(0, 5);      // Xóa phần đã xử lý khỏi resp_copy
            outputarrray.push_back(""); // Thêm chuỗi rỗng vào mảng kết quả
            continue;                   // chuyển sang phần tử tiếp theo
        }
        outputarrray.push_back(element); // Thêm phần tử vào mảng kết quả
        // Xóa phần đã xử lý khỏi resp_copy
        size_t element_len = element.length();
        size_t lenof_element_len = std::to_string(element_len).length();
        size_t total_erase_len = 1 + lenof_element_len + 2 + element_len + 2; // $ + len + \r\n + data + \r\n
        resp_copy.erase(0, total_erase_len);                                  // Xóa mảng đã xử lý khỏi resp_copy
    }
    return outputarrray; // Trả về mảng kết quả
}
// Tạo hàm pack map
std::string pack_map(std::vector<std::pair<std::string, std::string>> &data)
{
    size_t num_elements = data.size();
    std::string resp = "*" + std::to_string(num_elements) + "\r\n";
    for (const auto &key_value : data)
    {
        resp += pack_bulk_string(key_value.first);
        resp += pack_bulk_string(key_value.second);
    }
    return resp;
}
// Tạo hàm unpack map
std::vector<std::pair<std::string, std::string>> unpack_map(std::string resp)
{
    if (resp[0] != '%')
    {
        std::cerr << "Loi dong goi khong phu hop" << std::endl;
    }
    size_t pointer = resp.find(1);
    if (pointer == std::string::npos)
    {
        std::cerr << "Loi dinh dang phan giai" << std::endl;
    }
    std::string num_entries_str = resp.substr(1, pointer - 1);
    size_t num_entries;
    try
    {
        num_entries = std::stoll(num_entries_str);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Loi dinh dang phan giai (khong the tim kiem duoc cap key_value)" << e.what() << std::endl;
    }
    std::string resp_copy = resp;
    pointer += 2;
    resp_copy = resp_copy.erase(0, pointer);
    std::vector<std::pair<std::string, std::string>> output_map;
    for (int i = 0; i < num_entries; i++)
    {
        std::string key = unpack_bulk_string(resp_copy);
        resp_copy = resp_copy.erase(pointer, resp.find(pointer));
        std::string value = unpack_bulk_string(resp_copy);
        std::pair<std::string, std::string> key_value = std::make_pair(key, value);
        output_map.push_back(key_value);
    }
    return output_map;
}