#include "../include/LoginResponse.hxx"

#include "../include/Packager.hxx"

bool LoginResponse::get_response() const { return m_answ; }

std::vector<std::vector<char>> LoginResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_login_response(m_answ);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}