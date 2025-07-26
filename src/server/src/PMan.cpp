#include "../include/PMan.hxx"

#include "../include/Crypto.hxx"
#include "../include/Database.hxx"
#include "../include/Exception.hxx"

PMan* PMan::m_inst = nullptr;

void PMan::init() {
    if (PMan::m_inst == nullptr) {
        PMan::m_inst = new PMan;
    }
}
void PMan::kill() {
    if (PMan::m_inst != nullptr) {
        delete PMan::m_inst;
    }
    PMan::m_inst = nullptr;
}
PMan& PMan::get() {
    if (PMan::m_inst == nullptr) {
        throw Exception("PMan class not initialized", 0x01);
    }
    return *(PMan::m_inst);
}

std::vector<char> PMan::gen(std::vector<char> username,
                            int               len,
                            bool              special_chars,
                            bool              digits,
                            bool              capital_letters,
                            bool              non_capital_letters) {
    std::vector<char> password;
    char              character;
    int               curr_len = len;

    bool found_special = false, found_digit = false, found_upper = false,
         found_lower = false;
    while (curr_len) {
        bool keep_going = true;
        while (keep_going) {
            character =
                static_cast<char>(Crypto::get().random_int(33, 126));
            if (is_digit(character)) {
                keep_going = digits ? false : true;
            } else if (is_special(character)) {
                keep_going = special_chars ? false : true;
            } else if (is_upper(character)) {
                keep_going = capital_letters ? false : true;
            } else if (is_lower(character)) {
                keep_going = non_capital_letters ? false : true;
            }
        }

        if (is_digit(character)) {
            found_digit = true;
        } else if (is_special(character)) {
            found_special = true;
        } else if (is_upper(character)) {
            found_upper = true;
        } else if (is_lower(character)) {
            found_lower = true;
        }

        password.push_back(character);
        curr_len--;

        if (curr_len == 0) {
            if (found_digit != digits || found_special != special_chars ||
                found_upper != capital_letters ||
                found_lower != non_capital_letters) {
                curr_len = len;
                found_special = false, found_digit = false,
                found_upper = false, found_lower = false;
                password.clear();
            }
        }
    }

    return password;
}
std::vector<char> PMan::enc(std::vector<char> pass,
                            std::vector<char> username) {
    std::vector<char> enc_pass_hash =
        Database::get().get_password_hash(username);
    std::vector<char> decr_pass_hash =
        Crypto::get().aes_256_cbc_decr(enc_pass_hash);
    std::vector<char> enc_salt = Database::get().get_user_salt(username);
    std::vector<char> dec_salt = Crypto::get().aes_256_cbc_decr(enc_salt);

    std::vector<char> pbkdf;
    Crypto::get().gen_pbkdf2(std::string(decr_pass_hash.data()),
                             dec_salt,
                             dec_salt.size(),
                             dec_salt.size(),
                             pbkdf);

    std::vector<char> aes_key;
    std::vector<char> aes_iv;
    aes_key = Crypto::get().gen_aes_key(pbkdf);
    aes_iv = Crypto::get().gen_aes_iv(aes_key);

    std::vector<char> enc_pass =
        Crypto::get().aes_256_cbc_encr(pass, aes_key, aes_iv);

    return enc_pass;
}

bool PMan::is_special(char ch) const {
    return (ch >= 33 && ch <= 47) || (ch >= 91 && ch <= 96) ||
           (ch >= 123 && ch <= 126);
}
bool PMan::is_upper(char ch) const { return ch >= 65 && ch <= 90; }
bool PMan::is_lower(char ch) const { return ch >= 97 && ch <= 122; }
bool PMan::is_digit(char ch) const { return ch >= 48 && ch <= 57; }