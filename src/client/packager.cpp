#include "packager.h"
#include "crypto.h"
#include "loginmanager.h"
#include <QDebug>

Packager* Packager::m_inst = nullptr;

void Packager::init() {
    if (Packager::m_inst == nullptr){
        Packager::m_inst = new Packager();
    }
}
Packager& Packager::get() {
    return *(Packager::m_inst);
}
void Packager::kill() {
    if(Packager::m_inst != nullptr) {
        delete Packager::m_inst;
    }
    Packager::m_inst = nullptr;
}

std::vector<char> Packager::package_login_data(std::string username,
                                               std::string password) {
    std::vector<char> payload;

    payload.push_back(PAYLOAD::LOGIN);
    payload.push_back(username.size());
    for (int i = 0; i < username.size(); i++) {
        payload.push_back(username[i]);
    }

    std::string hash = Crypto::get().sha3_512(password);
    Crypto::get().set_user_hash(hash);

    payload.push_back(hash.size());
    for(int i = 0; i < hash.size(); i++) {
        payload.push_back(hash[i]);
    }

    return payload;
}

std::vector<char> Packager::package_register_data(std::string username,
                                                  std::string password,
                                                  std::string email) {
    std::vector<char> payload;

    payload.push_back(PAYLOAD::REGISTER);
    payload.push_back(username.size());
    for (int i = 0; i < username.size(); i++) {
        payload.push_back(username[i]);
    }

    std::string hash = Crypto::get().sha3_512(password);

    payload.push_back(hash.size());
    for(int i = 0; i < hash.size(); i++) {
        payload.push_back(hash[i]);
    }

    std::vector<char> salt = Crypto::get().generate_random_array();

    payload.push_back(salt.size());
    for(int i = 0; i < salt.size(); i++) {
        payload.push_back(salt[i]);
    }

    payload.push_back(email.size());
    for(int i = 0; i < email.size(); i++) {
        payload.push_back(email[i]);
    }

    return payload;
}

std::vector<char> Packager::package_salt_request(std::string username) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::SALT_REQUEST);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) {
        payload.push_back(username[i]);
    }
    return payload;
}

std::vector<char> Packager::package_pass_gen_data(
    int len,
    bool special_ch_flag,
    bool digit_flag,
    bool capital_flag,
    bool non_capital_flag,
    const std::string& logged_user,
    const std::string& username,
    const std::string& platform) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::GEN_PASS);
    payload.push_back(logged_user.size());
    for(int i = 0; i < logged_user.size(); i++) {
        payload.push_back(logged_user[i]);
    }
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) {
        payload.push_back(username[i]);
    }
    payload.push_back(len);
    payload.push_back(platform.size());
    for(int i = 0; i < platform.size(); i++){
        payload.push_back(platform[i]);
    }
    payload.push_back(special_ch_flag);
    payload.push_back(digit_flag);
    payload.push_back(capital_flag);
    payload.push_back(non_capital_flag);
    return payload;
}

std::vector<char> Packager::package_password_requests(std::string username) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::PASS_REQUEST);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) {
        payload.push_back(username[i]);
    }
    return payload;
}

std::vector<char> Packager::package_pass_del(std::string username, std::string password) {
    std::vector<char> plain_text;
    for(int i = 0; i < password.size(); i++) plain_text.push_back(password[i]);
    std::vector<char> client_side_enc_pass = Crypto::get().aes_256_cbc_encr(plain_text);

    std::vector<char> payload;
    payload.push_back(PAYLOAD::PASS_DEL);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) payload.push_back(username[i]);
    payload.push_back(client_side_enc_pass.size());
    for(int i = 0; i < client_side_enc_pass.size(); i++) {
        payload.push_back(client_side_enc_pass[i]);
    }
    return payload;
}

std::vector<char> Packager::package_email_change_data(std::string username, std::string email) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::EMAIL_CHANGE);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) payload.push_back(username[i]);
    payload.push_back(email.size());
    for(int i = 0; i < email.size(); i++) payload.push_back(email[i]);
    return payload;
}

std::vector<char> Packager::package_pass_change_data(std::string username, std::string pass) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::PASS_CHANGE);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) payload.push_back(username[i]);
    std::string hash = Crypto::get().sha3_512(pass);
    payload.push_back(hash.size());
    for(int i = 0; i < hash.size(); i++) payload.push_back(hash[i]);
    return payload;
}

std::vector<char> Packager::package_recovery_code_request(std::string username) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::RECOVERY_CODE_REQUEST);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) payload.push_back(username[i]);
    return payload;
}

std::vector<char> Packager::package_recovery_code_check(std::string username, std::string code) {
    std::vector<char> payload;
    payload.push_back(PAYLOAD::RECOVERY_CODE_CHECK);
    payload.push_back(username.size());
    for(int i = 0; i < username.size(); i++) payload.push_back(username[i]);
    payload.push_back(code.size());
    for(int i = 0; i < code.size(); i++) payload.push_back(code[i]);
    return payload;
}
