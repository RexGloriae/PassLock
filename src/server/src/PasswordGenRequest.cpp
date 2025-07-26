#include "../include/PasswordGenRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

PasswordGenRequest::PasswordGenRequest(
    const std::vector<char>& username,
    const std::vector<char>& platform_username,
    const std::vector<char>& platform,
    bool                     special_chars,
    bool                     digits,
    bool                     capital_letters,
    bool                     non_capital_letters,
    int                      len)
    : Request() {
    m_username = username;
    m_platform_username = platform_username;
    m_platform = platform;
    m_special_chars = special_chars;
    m_digits = digits;
    m_capital_letters = capital_letters;
    m_non_capital_letters = non_capital_letters;
    m_len = len;
}

IResponse* PasswordGenRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    std::vector<char> enc_platform =
        Crypto::get().aes_256_cbc_encr(m_platform);
    std::vector<char> enc_platform_username =
        Crypto::get().aes_256_cbc_encr(m_platform_username);

    m_username = enc_username;
    m_platform_username = enc_platform_username;
    m_platform = enc_platform;

    return Resolver::get().solve(
        dynamic_cast<const PasswordGenRequest&>(*this));
}

std::vector<char> PasswordGenRequest::get_username() const {
    return m_username;
}
std::vector<char> PasswordGenRequest::get_platform() const {
    return m_platform;
}
std::vector<char> PasswordGenRequest::get_platform_username() const {
    return m_platform_username;
}
bool PasswordGenRequest::get_special_chars() const {
    return m_special_chars;
}
bool PasswordGenRequest::get_digits() const { return m_digits; }
bool PasswordGenRequest::get_capital_letters() const {
    return m_capital_letters;
}
bool PasswordGenRequest::get_non_capital_letters() const {
    return m_non_capital_letters;
}
int PasswordGenRequest::get_pass_len() const { return m_len; }
