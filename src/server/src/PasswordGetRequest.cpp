#include "../include/PasswordGetRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

PasswordGetRequest::PasswordGetRequest(const std::vector<char>& username)
    : Request() {
    m_username = username;
}

IResponse* PasswordGetRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    m_username = enc_username;

    return Resolver::get().solve(
        dynamic_cast<const PasswordGetRequest&>(*this));
}

std::vector<char> PasswordGetRequest::get_username() const {
    return m_username;
}