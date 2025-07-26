#include "../include/Packager.hxx"

#include <iostream>

Packager* Packager::m_inst = nullptr;

void Packager::init() {
    if (Packager::m_inst == nullptr) {
        Packager::m_inst = new Packager();
    }
}
Packager& Packager::get() { return *(Packager::m_inst); }
void      Packager::kill() {
    if (Packager::m_inst != nullptr) {
        delete Packager::m_inst;
    }
    Packager::m_inst = nullptr;
}

std::vector<char> Packager::encapsulate_login_response(bool answ) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::LOGIN);
    switch (answ) {
        case true:
            payload.push_back(0x01);
            break;
        case false:
            payload.push_back(0x00);
            break;
    }
    return payload;
}

std::vector<char> Packager::encapsulate_register_response(bool answ) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::REGISTER);
    switch (answ) {
        case true:
            payload.push_back(0x01);
            break;
        case false:
            payload.push_back(0x00);
            break;
    }
    return payload;
}

std::vector<char> Packager::encapsulate_salt_fetch_response(
    const std::vector<char>& salt) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::SALT_REQUEST);
    payload.push_back(salt.size());
    for (int i = 0; i < salt.size(); i++) {
        payload.push_back(salt[i]);
    }
    return payload;
}

std::vector<char> Packager::encapsulate_password_gen_response(
    const std::vector<char>& pass) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::GEN_PASS);
    payload.push_back(pass.size());
    for (int i = 0; i < pass.size(); i++) {
        payload.push_back(pass[i]);
    }
    return payload;
}

std::vector<char> Packager::encapsulate_password_get_response(
    const std::tuple<std::vector<char>,
                     std::vector<char>,
                     std::vector<char>>& pass) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::SEND_PASS);
    payload.push_back(std::get<0>(pass).size());
    for (int i = 0; i < std::get<0>(pass).size(); i++) {
        payload.push_back(std::get<0>(pass)[i]);
    }
    payload.push_back(std::get<1>(pass).size());
    for (int i = 0; i < std::get<1>(pass).size(); i++) {
        payload.push_back(std::get<1>(pass)[i]);
    }
    payload.push_back(std::get<2>(pass).size());
    for (int i = 0; i < std::get<2>(pass).size(); i++) {
        payload.push_back(std::get<2>(pass)[i]);
    }
    return payload;
}

std::vector<char> Packager::encapsulate_email_change_response(bool answ) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::EMAIL_CHANGE);
    payload.push_back(answ);
    return payload;
}

std::vector<char> Packager::encapsulate_recovery_code_fetch_response(
    bool answ) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::RECOVERY_CODE_REQUEST);
    payload.push_back(answ);
    return payload;
}

std::vector<char> Packager::encapsulate_recovery_code_check_response(
    bool answer) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::RECOVERY_CODE_CHECK);
    payload.push_back(answer);
    return payload;
}
