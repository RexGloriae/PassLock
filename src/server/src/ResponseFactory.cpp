#include "../include/ResponseFactory.hxx"

IResponse* ResponseFactory::create_login_response(bool status) {
    IResponse* response = new LoginResponse(status);
    return response;
}
IResponse* ResponseFactory::create_register_response(bool status) {
    IResponse* response = new RegisterResponse(status);
    return response;
}
IResponse* ResponseFactory::create_salt_fetch_response(
    const std::vector<char>& salt) {
    IResponse* response = new SaltFetchResponse(salt);
    return response;
}
IResponse* ResponseFactory::create_pass_gen_response(
    const std::vector<char>& pass) {
    IResponse* response = new PasswordGenResponse(pass);
    return response;
}

IResponse* ResponseFactory::create_pass_get_response(
    const std::vector<std::tuple<std::vector<char>,
                                 std::vector<char>,
                                 std::vector<char>>>& passwords) {
    IResponse* response = new PasswordGetResponse(passwords);
    return response;
}

IResponse* ResponseFactory::create_email_change_response(bool status) {
    IResponse* response = new EmailChangeResponse(status);
    return response;
}

IResponse* ResponseFactory::create_recovery_code_fetch_response(
    bool status) {
    IResponse* response = new RecoveryCodeFetchResponse(status);
    return response;
}

IResponse* ResponseFactory::create_recovery_code_check_response(
    bool status) {
    IResponse* response = new RecoveryCodeCheckResponse(status);
    return response;
}