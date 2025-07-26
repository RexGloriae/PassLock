#ifndef EXCEPTION_HXX
#define EXCEPTION_HXX

#include <string>

class Exception {
   private:
    std::string m_mssg;
    int         m_code;

   public:
    Exception(std::string mess, int code);
    std::string what() const;
    int         code() const;
};

#endif  // EXCEPTION_HXX