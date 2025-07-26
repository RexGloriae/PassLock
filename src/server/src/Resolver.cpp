#include "../include/Resolver.hxx"

#include "../include/Crypto.hxx"
#include "../include/Database.hxx"
#include "../include/EmailChangeRequest.hxx"
#include "../include/Exception.hxx"
#include "../include/LoginRequest.hxx"
#include "../include/Mail.hxx"
#include "../include/PMan.hxx"
#include "../include/PasswordChangeRequest.hxx"
#include "../include/PasswordDelRequest.hxx"
#include "../include/PasswordGenRequest.hxx"
#include "../include/PasswordGetRequest.hxx"
#include "../include/RecoveryCodeCheckRequest.hxx"
#include "../include/RecoveryCodeFetchRequest.hxx"
#include "../include/RegisterRequest.hxx"
#include "../include/ResponseFactory.hxx"
#include "../include/SaltFetchRequest.hxx"

Resolver* Resolver::m_inst = nullptr;

void Resolver::init() {
    if (Resolver::m_inst == nullptr) {
        Resolver::m_inst = new Resolver();
    }
}
Resolver& Resolver::get() { return *(Resolver::m_inst); }
void      Resolver::kill() {
    if (Resolver::m_inst != nullptr) {
        delete Resolver::m_inst;
    }
    Resolver::m_inst = nullptr;
}

IResponse* Resolver::solve(const LoginRequest& req) {
    std::vector<char> username = req.get_username();
    std::vector<char> hash = Database::get().get_password_hash(username);
    bool              answer = (hash == req.get_pass_hash());
    Logger::get().write("A Login Request has been successfully solved.");
    if (answer == true) {
        Logger::get().write("User has successfully logged in.");
    } else {
        Logger::get().write("User has failed to log in.");
    }
    return ResponseFactory::create_login_response(answer);
}
IResponse* Resolver::solve(const RegisterRequest& req) {
    std::vector<char> username = req.get_username();
    std::vector<char> hash = req.get_pass_hash();
    std::vector<char> salt = req.get_salt();
    std::vector<char> email = req.get_email();
    bool              answ = true;
    std::vector<char> recovery_code = gen_recovery_code();
    std::vector<char> enc_rec_code =
        Crypto::get().aes_256_cbc_encr(recovery_code);

    Logger::get().write(
        "A Register Request has been successfully solved.");
    try {
        Database::get().add_new_user(
            username, hash, salt, email, enc_rec_code);
    } catch (const Exception& E) {
        answ = false;
        Logger::get().write("User has failed to register.");
    }
    if (answ == true) {
        std::string dest =
            std::string(Crypto::get().aes_256_cbc_decr(email).data());
        Mail::get().sendWelcomeMessage(dest);
        Logger::get().write("User has successfully registered.");
    }
    return ResponseFactory::create_register_response(answ);
}

IResponse* Resolver::solve(const SaltFetchRequest& req) {
    std::vector<char> username = req.get_username();
    std::vector<char> salt = Database::get().get_user_salt(username);

    std::vector<char> decr_salt = Crypto::get().aes_256_cbc_decr(salt);

    Logger::get().write(
        "A Salt Fetch Request has been successfully solved.");
    return ResponseFactory::create_salt_fetch_response(decr_salt);
}

IResponse* Resolver::solve(const PasswordGenRequest& req) {
    std::vector<char> username = req.get_username();
    std::vector<char> platform_username = req.get_platform_username();
    std::vector<char> platform = req.get_platform();
    bool              special_chars = req.get_special_chars();
    bool              digits = req.get_digits();
    bool              capital_letters = req.get_capital_letters();
    bool              non_capital_letters = req.get_non_capital_letters();
    int               len = req.get_pass_len();

    std::vector<char> password;
    bool              collided = false;
    if (Database::get().is_collision(
            username, platform, platform_username) == false) {
        password = PMan::get().gen(username,
                                   len,
                                   special_chars,
                                   digits,
                                   capital_letters,
                                   non_capital_letters);
    } else {
        password = set_collision();
        collided = true;
    }

    password = PMan::get().enc(password, username);

    if (collided == false) {
        std::vector<char> sv_side_enc_pass =
            Crypto::get().aes_256_cbc_encr(password);

        Database::get().insert_new_password(
            username, platform_username, platform, sv_side_enc_pass);

        Logger::get().write("A new password has been generated.");
    } else {
        Logger::get().write(
            "A new password coult not have been generated because of a "
            "collision.");
    }

    Logger::get().write(
        "A Password Generation Request has been successfully solved.");
    return ResponseFactory::create_pass_gen_response(password);
}

IResponse* Resolver::solve(const PasswordGetRequest& req) {
    std::vector<char> username = req.get_username();

    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>

        passwords = Database::get().get_all_passwords_for_user(username);

    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>
        decr_passwords;
    std::tuple<std::vector<char>, std::vector<char>, std::vector<char>>
                      decr_entry;
    std::vector<char> decr_pass_value, decr_platform, decr_username;

    for (int i = 0; i < passwords.size(); i++) {
        decr_platform =
            Crypto::get().aes_256_cbc_decr(std::get<0>(passwords[i]));
        decr_pass_value =
            Crypto::get().aes_256_cbc_decr(std::get<1>(passwords[i]));
        decr_username =
            Crypto::get().aes_256_cbc_decr(std::get<2>(passwords[i]));

        decr_passwords.push_back(std::make_tuple(
            decr_pass_value, decr_platform, decr_username));
    }

    Logger::get().write(
        "A Password Fetch Request has been successfully solved.");
    return ResponseFactory::create_pass_get_response(decr_passwords);
}

IResponse* Resolver::solve(const PasswordDelRequest& req) {
    std::vector<char> user = req.get_username();
    std::vector<char> pass = req.get_stored_pass();

    Database::get().delete_password(user, pass);

    Logger::get().write(
        "A Password Deletion Request has been successfully solved.");
    return nullptr;
}

IResponse* Resolver::solve(const EmailChangeRequest& req) {
    std::vector<char> user = req.get_username();
    std::vector<char> email = req.get_email();
    bool              answ = true;
    try {
        Database::get().update_user_email(user, email);
    } catch (const Exception& E) {
        answ = false;
        Logger::get().write(
            "Email could not have been changed - already in use.");
    }

    if (answ == true) {
        std::string dest =
            std::string(Crypto::get().aes_256_cbc_decr(email).data());
        Mail::get().sendChangedEmailMessage(dest);
        Logger::get().write("Email was changed.");
    }

    Logger::get().write(
        "An Email Change Request has been successfully solved.");
    return ResponseFactory::create_email_change_response(answ);
}

IResponse* Resolver::solve(const PasswordChangeRequest& req) {
    change_pass(req.get_username(), req.get_pass_hash());
    Logger::get().write(
        "A Password Change Request has been successfully solved.");
    return nullptr;
}

IResponse* Resolver::solve(const RecoveryCodeFetchRequest& req) {
    std::vector<char> username = req.get_username();

    bool answ = true;
    try {
        std::vector<char> code =
            Database::get().get_user_recovery_code(username);
        code = Crypto::get().aes_256_cbc_decr(code);

        std::vector<char> email = Database::get().get_user_email(username);
        std::string       dest =
            std::string(Crypto::get().aes_256_cbc_decr(email).data());
        Mail::get().sendRecoveryCode(dest, std::string(code.data()));
    } catch (const Exception& E) {
        answ = false;
    }

    Logger::get().write(
        "A Recovery Code Fetch Request has been successfully solved.");
    return ResponseFactory::create_recovery_code_fetch_response(answ);
}

IResponse* Resolver::solve(const RecoveryCodeCheckRequest& req) {
    std::vector<char> user = req.get_username();
    std::vector<char> code = req.get_code();

    std::vector<char> db_code =
        Database::get().get_user_recovery_code(user);

    bool answ = true;
    for (int i = 0; i < db_code.size(); i++) {
        if (db_code[i] != code[i]) answ = false;
    }

    if (answ == true) {
        std::vector<char> new_code = gen_recovery_code();
        new_code = Crypto::get().aes_256_cbc_encr(new_code);
        Database::get().add_recovery_code(user, new_code);
    }

    Logger::get().write(
        "A Recovery Code Check Request has been successfully solved.");
    return ResponseFactory::create_recovery_code_check_response(answ);
}

std::vector<char> Resolver::gen_recovery_code() {
    int               int_rec = Crypto::get().random_int(1000, 9999);
    std::string       str_rec = std::to_string(int_rec);
    std::vector<char> vec_rec;
    for (int i = 0; i < str_rec.size(); i++) vec_rec.push_back(str_rec[i]);
    return vec_rec;
}

std::vector<char> Resolver::set_collision() {
    std::vector<char> coll;
    coll.push_back('C');
    coll.push_back('o');
    coll.push_back('l');
    coll.push_back('l');
    coll.push_back('i');
    coll.push_back('s');
    coll.push_back('i');
    coll.push_back('o');
    coll.push_back('n');
    return coll;
}

void Resolver::change_pass(const std::vector<char>& user,
                           const std::vector<char>& new_pass) {
    std::vector<char> salt = Database::get().get_user_salt(user);
    std::vector<char> email = Database::get().get_user_email(user);
    std::vector<char> recovery =
        Database::get().get_user_recovery_code(user);
    std::vector<char> old_pass = Database::get().get_password_hash(user);
    old_pass = Crypto::get().aes_256_cbc_decr(old_pass);
    std::vector<char> decr_salt = Crypto::get().aes_256_cbc_decr(salt);
    std::vector<char> old_pbkdf;
    Crypto::get().gen_pbkdf2(std::string(old_pass.data()),
                             decr_salt,
                             decr_salt.size(),
                             decr_salt.size(),
                             old_pbkdf);
    std::vector<char> old_key = Crypto::get().gen_aes_key(old_pbkdf);
    std::vector<char> old_iv = Crypto::get().gen_aes_iv(old_key);

    std::vector<char> new_pbkdf;
    std::string       str_new_pass;
    for (int i = 0; i < new_pass.size(); i++) {
        str_new_pass.push_back(new_pass[i]);
    }
    Crypto::get().gen_pbkdf2(str_new_pass,
                             decr_salt,
                             decr_salt.size(),
                             decr_salt.size(),
                             new_pbkdf);
    std::vector<char> new_key = Crypto::get().gen_aes_key(new_pbkdf);
    std::vector<char> new_iv = Crypto::get().gen_aes_iv(new_key);

    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>
        passwords = Database::get().get_all_passwords_for_user(user);

    Database::get().delete_user(user);
    std::vector<char> enc_new_pass =
        Crypto::get().aes_256_cbc_encr(new_pass);
    Database::get().add_new_user(
        user, enc_new_pass, salt, email, recovery);

    for (int i = 0; i < passwords.size(); i++) {
        std::vector<char>& curr_pass = std::get<1>(passwords[i]);
        curr_pass = Crypto::get().aes_256_cbc_decr(curr_pass);
        curr_pass =
            Crypto::get().aes_256_cbc_decr(curr_pass, old_key, old_iv);
        curr_pass =
            Crypto::get().aes_256_cbc_encr(curr_pass, new_key, new_iv);
        curr_pass = Crypto::get().aes_256_cbc_encr(curr_pass);

        Database::get().insert_new_password(user,
                                            std::get<2>(passwords[i]),
                                            std::get<0>(passwords[i]),
                                            curr_pass);
    }
}