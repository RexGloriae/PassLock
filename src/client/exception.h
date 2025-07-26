#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception {
private:
    std::string m_mssg;
    int         m_code;

    std::string stringify_code(int code) const;

public:
    Exception(std::string mess, int code);
    std::string what() const;
};

#endif  // EXCEPTION_HXX
