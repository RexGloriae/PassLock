#include "../include/RequestFactory.hxx"

#include "../include/Logger.hxx"

IRequest* RequestFactory::create_login_request(
    const std::vector<char>& username, const std::vector<char>& pass) {
    IRequest* request = new LoginRequest(username, pass);
    Logger::get().write("A new Login Request has been created.");
    return request;
}

IRequest* RequestFactory::create_register_request(
    const std::vector<char>& username,
    const std::vector<char>& pass,
    const std::vector<char>& salt,
    const std::vector<char>& email) {
    IRequest* request = new RegisterRequest(username, pass, salt, email);
    Logger::get().write("A new Register Request has been created.");
    return request;
}

IRequest* RequestFactory::create_salt_fetch_request(
    const std::vector<char>& username) {
    IRequest* request = new SaltFetchRequest(username);
    Logger::get().write("A new Salt Fetch Request has been created.");
    return request;
}

IRequest* RequestFactory::create_password_gen_request(
    const std::vector<char>& username,
    const std::vector<char>& platform_username,
    const std::vector<char>& platform,
    bool                     special_chars_flag,
    bool                     digit_flag,
    bool                     capital_flag,
    bool                     non_capital_flag,
    int                      len) {
    IRequest* request = new PasswordGenRequest(username,
                                               platform_username,
                                               platform,
                                               special_chars_flag,
                                               digit_flag,
                                               capital_flag,
                                               non_capital_flag,
                                               len);
    Logger::get().write(
        "A new Password Generation Request has been created.");
    return request;
}

IRequest* RequestFactory::create_password_get_request(
    const std::vector<char>& username) {
    IRequest* request = new PasswordGetRequest(username);
    Logger::get().write("A new Password Fetch Request has been created.");
    return request;
}

IRequest* RequestFactory::create_password_del_request(
    const std::vector<char>& username, const std::vector<char>& pass) {
    IRequest* request = new PasswordDelRequest(username, pass);
    Logger::get().write(
        "A new Password Deletion Request has been created.");
    return request;
}

IRequest* RequestFactory::create_email_change_request(
    const std::vector<char>& user, const std::vector<char>& email) {
    IRequest* request = new EmailChangeRequest(user, email);
    Logger::get().write("A new Email Change Request has been created.");
    return request;
}

IRequest* RequestFactory::create_pass_change_request(
    const std::vector<char>& user, const std::vector<char>& hash) {
    IRequest* request = new PasswordChangeRequest(user, hash);
    Logger::get().write("A new Password Change Request has been created.");
    return request;
}

IRequest* RequestFactory::create_recovery_code_request(
    const std::vector<char>& user) {
    IRequest* request = new RecoveryCodeFetchRequest(user);
    Logger::get().write(
        "A new Recovery Code Fetch Request has been created.");
    return request;
}

IRequest* RequestFactory::create_recovery_code_check_request(
    const std::vector<char>& user, const std::vector<char>& code) {
    IRequest* request = new RecoveryCodeCheckRequest(user, code);
    Logger::get().write(
        "A new Recovery Code Check Request has been created.");
    return request;
}
