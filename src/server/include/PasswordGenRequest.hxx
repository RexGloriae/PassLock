#ifndef PASSWORD_GEN_REQUEST_HXX
#define PASSWORD_GEN_REQUEST_HXX

#include <vector>

#include "Request.hxx"

class PasswordGenRequest : public Request {
   private:
    std::vector<char> m_username;
    std::vector<char> m_platform_username;
    std::vector<char> m_platform;
    bool              m_special_chars;
    bool              m_digits;
    bool              m_capital_letters;
    bool              m_non_capital_letters;
    int               m_len;

   public:
    /**
     * @param username Username of the user who created this request.
     * @param platform_username Username to be associated with the
     * generated password.
     * @param platform Platform to be associated with the generated
     * password
     * @param special_chars Flag representing the usage of special
     * characters in the generated password.
     * @param digits Flag representing the usage of digits in the generated
     * password.
     * @param capital_letters Flag representing the usage of upper-case
     * letters in the generated password.
     * @param non_capital_letters Flag representing the usage of lower-case
     * letters in the generated password.
     * @param len Length of the generated password.
     */
    PasswordGenRequest(const std::vector<char>& username,
                       const std::vector<char>& platform_username,
                       const std::vector<char>& platform,
                       bool                     special_chars,
                       bool                     digits,
                       bool                     capital_letters,
                       bool                     non_capital_letters,
                       int                      len);

    IResponse* solve() override;

    std::vector<char> get_username() const override;
    std::vector<char> get_platform() const override;
    std::vector<char> get_platform_username() const override;
    bool              get_special_chars() const override;
    bool              get_digits() const override;
    bool              get_capital_letters() const override;
    bool              get_non_capital_letters() const override;
    int               get_pass_len() const override;
};

#endif  // PASSWORD_GEN_REQUEST_HXX