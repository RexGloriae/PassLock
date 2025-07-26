#include "../include/EmailChangeRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

EmailChangeRequest::EmailChangeRequest(const std::vector<char>& username,
                                       const std::vector<char>& email)
    : Request() {
    m_username = username;
    m_email = email;
}

IResponse* EmailChangeRequest::solve() {
    std::vector<char> enc_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    std::vector<char> enc_email = Crypto::get().aes_256_cbc_encr(m_email);
    m_email = enc_email;
    m_username = enc_username;

    return Resolver::get().solve(
        dynamic_cast<const EmailChangeRequest&>(*this));
}

std::vector<char> EmailChangeRequest::get_username() const {
    return m_username;
}

std::vector<char> EmailChangeRequest::get_email() const { return m_email; }