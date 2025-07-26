#include "../include/Response.hxx"

#include "../include/Exception.hxx"

std::vector<std::vector<char>> Response::encapsulate() {
    throw Exception("Not implemented", 0x02);
}
bool Response::get_response() const {
    throw Exception("Not implemented", 0x02);
}
std::vector<char> Response::get_salt() const {
    throw Exception("Not implemented", 0x02);
}