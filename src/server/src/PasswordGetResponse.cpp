#include "../include/PasswordGetResponse.hxx"

#include "../include/Packager.hxx"

std::vector<std::vector<char>> PasswordGetResponse::encapsulate() {
    std::vector<std::vector<char>> frame;
    std::vector<char>              payload;
    payload.push_back(PAYLOAD::SEND_PASS_START);
    payload.push_back(m_passwords.size());
    frame.push_back(payload);
    int count = m_passwords.size();
    for (int i = 0; i < count; i++) {
        payload = Packager::get().encapsulate_password_get_response(
            m_passwords[i]);
        frame.push_back(payload);
    }
    return frame;
}