#include "interpreter.h"
#include "crypto.h"

Interpreter* Interpreter::m_inst = nullptr;

void Interpreter::init() {
    if (Interpreter::m_inst == nullptr) {
        Interpreter::m_inst = new Interpreter();
    }
}
Interpreter& Interpreter::get() { return *(Interpreter::m_inst); }
void         Interpreter::kill() {
    if (Interpreter::m_inst != nullptr) {
        delete Interpreter::m_inst;
    }
    Interpreter::m_inst = nullptr;
}

std::vector<char> Interpreter::retrieve_salt_from_payload(
    const char* payload,
    int len) {
    int i = 1;
    char size = payload[i++];
    std::vector<char> salt;
    while(size--){
        salt.push_back(payload[i++]);
    }
    return salt;
}

QMap<QString, QString> Interpreter::retrieve_password_from_payload(const char* payload) {
    std::vector<char> enc_pass;
    int enc_pass_size;
    std::string platform, user;
    int i = 1;
    enc_pass_size = payload[i++];
    while(enc_pass_size--){
        enc_pass.push_back(payload[i++]);
    }
    std::vector<char> dec_pass = Crypto::get().aes_256_cbc_decr(enc_pass);

    int platform_size = payload[i++];
    while(platform_size--){
        platform.push_back(payload[i++]);
    }

    int user_size = payload[i++];
    while(user_size--){
        user.push_back(payload[i++]);
    }

    QMap<QString, QString> password;
    std::string str_dec_pass(dec_pass.data());
    password["password"] = QString::fromStdString(str_dec_pass);
    password["platform"] = QString::fromStdString(platform);
    password["username"] = QString::fromStdString(user);

    return password;
}
