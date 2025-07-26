#include "../include/SaltFetchRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

SaltFetchRequest::SaltFetchRequest(const std::vector<char>& username)
    : Request() {
    m_username = username;
}

std::vector<char> SaltFetchRequest::get_username() const {
    return m_username;
}
IResponse* SaltFetchRequest::solve() {
    std::vector<char> encr_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    m_username = encr_username;

    return Resolver::get().solve(
        dynamic_cast<const SaltFetchRequest&>(*this));
}