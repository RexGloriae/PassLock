#include "../include/RegisterResponse.hxx"

#include "../include/Packager.hxx"

bool RegisterResponse::get_response() const { return m_answ; }

std::vector<std::vector<char>> RegisterResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_register_response(m_answ);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}