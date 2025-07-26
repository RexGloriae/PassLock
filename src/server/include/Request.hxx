#ifndef REQUEST_HXX
#define REQUEST_HXX

#include "IRequest.hxx"

class Request : public IRequest {
   public:
    Request() = default;
    virtual IResponse*        solve() override;
    virtual std::vector<char> get_username() const override;
    virtual std::vector<char> get_code() const;
    virtual std::vector<char> get_email() const override;
    virtual std::vector<char> get_pass_hash() const override;
    virtual std::vector<char> get_stored_pass() const override;
    virtual std::vector<char> get_platform() const override;
    virtual std::vector<char> get_platform_username() const override;
    virtual bool              get_special_chars() const override;
    virtual bool              get_digits() const override;
    virtual bool              get_capital_letters() const override;
    virtual bool              get_non_capital_letters() const override;
    virtual int               get_pass_len() const override;
};

#endif  // REQUEST_HXX