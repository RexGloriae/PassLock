#include "Exception.h"

#include <cstring>

Exception::Exception(std::string mess, int code) {
    this->m_mssg = mess;
    this->m_code = code;
}

std::string Exception::what() const {
    std::string str = "Std Exception: ";
    str += this->m_mssg;
    str += ": " + this->stringify_code(this->m_code);
    return str;
}

std::string Exception::stringify_code(int code) const {
    char buff[16];
    snprintf(buff, 16, "%d", code);
    return std::string(buff);
}
