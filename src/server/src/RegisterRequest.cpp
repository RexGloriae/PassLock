#include "../include/RegisterRequest.hxx"

#include "../include/Crypto.hxx"
#include "../include/Resolver.hxx"

RegisterRequest::RegisterRequest(const std::vector<char>& username,
                                 const std::vector<char>& hash,
                                 const std::vector<char>& salt,
                                 const std::vector<char>& email)
    : Request() {
    m_pass_hash = hash;
    m_username = username;
    m_salt = salt;
    m_email = email;
}

IResponse* RegisterRequest::solve() {
    std::vector<char> encr_username =
        Crypto::get().aes_256_cbc_encr(m_username);
    std::vector<char> encr_pass_hash =
        Crypto::get().aes_256_cbc_encr(m_pass_hash);
    std::vector<char> encr_salt = Crypto::get().aes_256_cbc_encr(m_salt);
    std::vector<char> encr_email = Crypto::get().aes_256_cbc_encr(m_email);

    m_username = encr_username;
    m_pass_hash = encr_pass_hash;
    m_salt = encr_salt;
    m_email = encr_email;

    return Resolver::get().solve(
        dynamic_cast<const RegisterRequest&>(*this));
}

std::vector<char> RegisterRequest::get_username() const {
    return m_username;
}
std::vector<char> RegisterRequest::get_email() const { return m_email; }
std::vector<char> RegisterRequest::get_pass_hash() const {
    return m_pass_hash;
}
std::vector<char> RegisterRequest::get_salt() const { return m_salt; }