#include "../include/LoginRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

LoginRequest::LoginRequest(const std::vector<char>& username,
                           const std::vector<char>& hash)
    : Request() {
    m_pass_hash = hash;
    m_username = username;
}

std::vector<char> LoginRequest::get_username() const { return m_username; }
std::vector<char> LoginRequest::get_pass_hash() const {
    return m_pass_hash;
}

IResponse* LoginRequest::solve() {
    std::vector<char> encr_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    std::vector<char> encr_pass_hash =
        Crypto::get().aes_256_cbc_encr(m_pass_hash);

    m_username = encr_username;
    m_pass_hash = encr_pass_hash;
    return Resolver::get().solve(dynamic_cast<const LoginRequest&>(*this));
}