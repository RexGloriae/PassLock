#include "../include/SaltFetchResponse.hxx"

#include "../include/Packager.hxx"

SaltFetchResponse::SaltFetchResponse(const std::vector<char>& salt)
    : Response() {
    m_salt = salt;
}

std::vector<char> SaltFetchResponse::get_salt() const { return m_salt; }

std::vector<std::vector<char>> SaltFetchResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_salt_fetch_response(m_salt);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}