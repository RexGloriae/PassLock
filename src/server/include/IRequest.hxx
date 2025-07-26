#ifndef IREQUEST_HXX
#define IREQUEST_HXX

#include <vector>

#include "Logger.hxx"

class IResponse;

class IRequest {
   public:
    virtual ~IRequest() = default;

    virtual IResponse* solve() = 0;

    virtual std::vector<char> get_username() const = 0;
    virtual std::vector<char> get_code() const = 0;
    virtual std::vector<char> get_email() const = 0;
    virtual std::vector<char> get_pass_hash() const = 0;
    virtual std::vector<char> get_stored_pass() const = 0;
    virtual std::vector<char> get_platform() const = 0;
    virtual std::vector<char> get_platform_username() const = 0;
    virtual bool              get_special_chars() const = 0;
    virtual bool              get_digits() const = 0;
    virtual bool              get_capital_letters() const = 0;
    virtual bool              get_non_capital_letters() const = 0;
    virtual int               get_pass_len() const = 0;
};

#endif  // IREQUEST_HXX