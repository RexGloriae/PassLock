#include "../include/Interpreter.hxx"

#include <vector>

#include "../include/Exception.hxx"
#include "../include/RequestFactory.hxx"

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

IRequest* Interpreter::interpret_and_fetch(const char* payload, int len) {
    if (len <= 1) return nullptr;
    switch (payload[0]) {
        case PAYLOAD::LOGIN:
            return this->interpret_login_payload(payload, len);
            break;
        case PAYLOAD::REGISTER:
            return this->interpret_register_payload(payload, len);
            break;
        case PAYLOAD::SALT_REQUEST:
            return this->interpret_salt_request_payload(payload, len);
            break;
        case PAYLOAD::GEN_PASS:
            return this->interpret_pass_gen_payload(payload, len);
        case PAYLOAD::PASS_REQUEST:
            return this->interpret_pass_request_payload(payload, len);
            break;
        case PAYLOAD::PASS_DEL:
            return this->interpret_pass_del_payload(payload, len);
            break;
        case PAYLOAD::EMAIL_CHANGE:
            return this->interpret_email_change_payload(payload, len);
            break;
        case PAYLOAD::PASS_CHANGE:
            return this->interpret_pass_change_payload(payload, len);
            break;
        case PAYLOAD::RECOVERY_CODE_REQUEST:
            return this->interpret_recovery_code_request_payload(payload,
                                                                 len);
            break;
        case PAYLOAD::RECOVERY_CODE_CHECK:
            return this->interpret_recovery_code_check_payload(payload,
                                                               len);
        default:
            throw Exception("Wrong payload structure", 0x01);
    }
}

IRequest* Interpreter::interpret_login_payload(const char* payload,
                                               int         len) {
    unsigned char elem_len = static_cast<unsigned char>(payload[1]);
    int           i = 2;

    std::vector<char> username;
    while (elem_len--) {
        username.push_back(payload[i++]);
    }

    elem_len = static_cast<unsigned char>(payload[i++]);

    std::vector<char> hash;
    while (elem_len--) {
        hash.push_back(payload[i++]);
    }

    return RequestFactory::create_login_request(username, hash);
}

IRequest* Interpreter::interpret_register_payload(const char* payload,
                                                  int         len) {
    unsigned char elem_len = static_cast<unsigned char>(payload[1]);
    int           i = 2;

    std::vector<char> username;
    while (elem_len--) {
        username.push_back(payload[i++]);
    }

    elem_len = static_cast<unsigned char>(payload[i++]);
    std::vector<char> hash;
    while (elem_len--) {
        hash.push_back(payload[i++]);
    }

    std::vector<char> salt;
    elem_len = static_cast<unsigned char>(payload[i++]);
    while (elem_len--) {
        salt.push_back(payload[i++]);
    }
    elem_len = static_cast<unsigned char>(payload[i++]);
    std::vector<char> email;
    while (elem_len--) {
        email.push_back(payload[i++]);
    }

    return RequestFactory::create_register_request(
        username, hash, salt, email);
}

IRequest* Interpreter::interpret_salt_request_payload(const char* payload,
                                                      int         len) {
    unsigned char     elem_len = static_cast<unsigned char>(payload[1]);
    int               i = 2;
    std::vector<char> username;
    while (elem_len--) {
        username.push_back(payload[i++]);
    }

    return RequestFactory::create_salt_fetch_request(username);
}

IRequest* Interpreter::interpret_pass_gen_payload(const char* payload,
                                                  int         len) {
    unsigned char     elem_len = static_cast<unsigned char>(payload[1]);
    int               i = 2;
    std::vector<char> username;
    while (elem_len--) {
        username.push_back(payload[i++]);
    }
    elem_len = payload[i++];
    std::vector<char> user_platform;
    while (elem_len--) {
        user_platform.push_back(payload[i++]);
    }
    elem_len = payload[i++];
    int pass_len = static_cast<int>(elem_len);
    elem_len = payload[i++];
    std::vector<char> platform;
    while (elem_len--) {
        platform.push_back(payload[i++]);
    }
    bool special_chars_flag = payload[i++];
    bool digit_flag = payload[i++];
    bool capital_letter_flag = payload[i++];
    bool non_capital_letter_flag = payload[i++];

    return RequestFactory::create_password_gen_request(
        username,
        user_platform,
        platform,
        special_chars_flag,
        digit_flag,
        capital_letter_flag,
        non_capital_letter_flag,
        pass_len);
}

IRequest* Interpreter::interpret_pass_request_payload(const char* payload,
                                                      int         len) {
    std::vector<char> username;
    int               i = 1;
    int               username_len = payload[i++];
    while (username_len--) {
        username.push_back(payload[i++]);
    }

    return RequestFactory::create_password_get_request(username);
}

IRequest* Interpreter::interpret_pass_del_payload(const char* payload,
                                                  int         len) {
    int               username_len = payload[1];
    std::vector<char> username;
    int               i = 2;
    while (username_len--) username.push_back(payload[i++]);
    int               pass_len = payload[i++];
    std::vector<char> pass;
    while (pass_len--) pass.push_back(payload[i++]);

    return RequestFactory::create_password_del_request(username, pass);
}

IRequest* Interpreter::interpret_email_change_payload(const char* payload,
                                                      int         len) {
    int               el_len = payload[1];
    std::vector<char> username;
    int               i = 2;
    while (el_len--) username.push_back(payload[i++]);
    el_len = payload[i++];
    std::vector<char> email;
    while (el_len--) email.push_back(payload[i++]);

    return RequestFactory::create_email_change_request(username, email);
}

IRequest* Interpreter::interpret_pass_change_payload(const char* payload,
                                                     int         len) {
    unsigned char     elem_len = static_cast<unsigned char>(payload[1]);
    std::vector<char> username;
    int               i = 2;
    while (elem_len--) username.push_back(payload[i++]);
    elem_len = static_cast<unsigned char>(payload[i++]);
    std::vector<char> hash;
    while (elem_len--) hash.push_back(payload[i++]);

    return RequestFactory::create_pass_change_request(username, hash);
}

IRequest* Interpreter::interpret_recovery_code_request_payload(
    const char* payload, int len) {
    int               elem_len = payload[1];
    int               i = 2;
    std::vector<char> username;
    while (elem_len--) username.push_back(payload[i++]);

    return RequestFactory::create_recovery_code_request(username);
}

IRequest* Interpreter::interpret_recovery_code_check_payload(
    const char* payload, int len) {
    int               elem_len = payload[1];
    int               i = 2;
    std::vector<char> username;
    while (elem_len--) username.push_back(payload[i++]);
    elem_len = payload[i++];
    std::vector<char> code;
    while (elem_len--) code.push_back(payload[i++]);

    return RequestFactory::create_recovery_code_check_request(username,
                                                              code);
}