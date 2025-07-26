#include "../include/RecoveryCodeCheckRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

RecoveryCodeCheckRequest::RecoveryCodeCheckRequest(
    const std::vector<char>& username, const std::vector<char>& code)
    : Request() {
    m_username = username;
    m_code = code;
}

IResponse* RecoveryCodeCheckRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    std::vector<char> enc_code = Crypto::get().aes_256_cbc_encr(m_code);
    m_username = enc_username;
    m_code = enc_code;

    return Resolver::get().solve(
        dynamic_cast<const RecoveryCodeCheckRequest&>(*this));
}

std::vector<char> RecoveryCodeCheckRequest::get_username() const {
    return m_username;
}

std::vector<char> RecoveryCodeCheckRequest::get_code() const {
    return m_code;
}