#include "../include/PasswordGenResponse.hxx"

#include "../include/Packager.hxx"

std::vector<std::vector<char>> PasswordGenResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_password_gen_response(m_pass);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}