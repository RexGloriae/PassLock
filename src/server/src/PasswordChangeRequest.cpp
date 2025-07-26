#include "../include/PasswordChangeRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

PasswordChangeRequest::PasswordChangeRequest(
    const std::vector<char>& username, const std::vector<char>& hash)
    : Request() {
    m_username = username;
    m_hash = hash;
}

IResponse* PasswordChangeRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    m_username = enc_username;

    return Resolver::get().solve(
        dynamic_cast<const PasswordChangeRequest&>(*this));
}

std::vector<char> PasswordChangeRequest::get_username() const {
    return m_username;
}

std::vector<char> PasswordChangeRequest::get_pass_hash() const {
    return m_hash;
}