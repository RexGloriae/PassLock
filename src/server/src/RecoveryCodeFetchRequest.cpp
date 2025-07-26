#include "../include/RecoveryCodeFetchRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

RecoveryCodeFetchRequest::RecoveryCodeFetchRequest(
    const std::vector<char>& username)
    : Request() {
    m_username = username;
}

IResponse* RecoveryCodeFetchRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    m_username = enc_username;

    return Resolver::get().solve(
        dynamic_cast<const RecoveryCodeFetchRequest&>(*this));
}

std::vector<char> RecoveryCodeFetchRequest::get_username() const {
    return m_username;
}