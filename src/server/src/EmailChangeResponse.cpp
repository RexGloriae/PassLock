#include "../include/EmailChangeResponse.hxx"

#include "../include/Packager.hxx"

std::vector<std::vector<char>> EmailChangeResponse::encapsulate() {
    std::vector<char> payload =
        Packager::get().encapsulate_email_change_response(m_status);

    std::vector<std::vector<char>> frame;
    frame.push_back(payload);
    return frame;
}