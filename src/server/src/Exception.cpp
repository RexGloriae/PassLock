#include "../include/Exception.hxx"

#include <cstring>

Exception::Exception(std::string mess, int code) {
    this->m_mssg = mess;
    this->m_code = code;
}

std::string Exception::what() const {
    std::string str = "Std Exception: ";
    str += this->m_mssg;
    return str;
}

int Exception::code() const { return this->m_code; }