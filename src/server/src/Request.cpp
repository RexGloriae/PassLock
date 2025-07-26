#include "../include/Request.hxx"

#include "../include/Exception.hxx"

IResponse* Request::solve() { throw Exception("Not implemented", 0x02); };

std::vector<char> Request::get_username() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_code() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_email() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_pass_hash() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_stored_pass() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_platform() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Request::get_platform_username() const {
    throw Exception("Not implemented", 0x02);
}
bool Request::get_special_chars() const {
    throw Exception("Not implemented", 0x02);
}
bool Request::get_digits() const {
    throw Exception("Not implemented", 0x02);
}
bool Request::get_capital_letters() const {
    throw Exception("Not implemented", 0x02);
}
bool Request::get_non_capital_letters() const {
    throw Exception("Not implemented", 0x02);
}
int Request::get_pass_len() const {
    throw Exception("Not implemented", 0x02);
}