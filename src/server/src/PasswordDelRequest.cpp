#include "../include/PasswordDelRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

PasswordDelRequest::PasswordDelRequest(const std::vector<char>& username,
                                       const std::vector<char>& pass)
    : Request() {
    m_username = username;
    m_stored_pass = pass;
}

std::vector<char> PasswordDelRequest::get_username() const {
    return m_username;
}

std::vector<char> PasswordDelRequest::get_stored_pass() const {
    return m_stored_pass;
}

IResponse* PasswordDelRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    m_username = enc_username;
    std::vector<char> enc_pass =
        Crypto::get().aes_256_cbc_encr(m_stored_pass);
    m_stored_pass = enc_pass;

    return Resolver::get().solve(
        dynamic_cast<const PasswordDelRequest&>(*this));
}