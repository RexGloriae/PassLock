#ifndef REQUESTFACTORY_HXX
#define REQUESTFACTORY_HXX

#include "EmailChangeRequest.hxx"
#include "LoginRequest.hxx"
#include "PasswordChangeRequest.hxx"
#include "PasswordDelRequest.hxx"
#include "PasswordGenRequest.hxx"
#include "PasswordGetRequest.hxx"
#include "RecoveryCodeCheckRequest.hxx"
#include "RecoveryCodeFetchRequest.hxx"
#include "RegisterRequest.hxx"
#include "SaltFetchRequest.hxx"

class RequestFactory {
   public:
    /**
     * @param username Username of the user creating this request.
     * @param hash Hash of the password used when trying to login.
     */
    static IRequest* create_login_request(
        const std::vector<char>& username, const std::vector<char>& pass);

    /**
     * @param username Username of the user who created this request.
     * @param hash Hash of the password of the user.
     * @param salt Salt generated for client-side encryption.
     * @param email Email of the user.
     */
    static IRequest* create_register_request(
        const std::vector<char>& username,
        const std::vector<char>& pass,
        const std::vector<char>& salt,
        const std::vector<char>& email);

    /**
     * @param username Username of the user who created this request.
     */
    static IRequest* create_salt_fetch_request(
        const std::vector<char>& username);

    /**
     * @param username Username of the user who created this request.
     * @param platform_username Username to be associated with the
     * generated password.
     * @param platform Platform to be associated with the generated
     * password
     * @param special_chars Flag representing the usage of special
     * characters in the generated password.
     * @param digits Flag representing the usage of digits in the generated
     * password.
     * @param capital_letters Flag representing the usage of upper-case
     * letters in the generated password.
     * @param non_capital_letters Flag representing the usage of lower-case
     * letters in the generated password.
     * @param len Length of the generated password.
     */
    static IRequest* create_password_gen_request(
        const std::vector<char>& username,
        const std::vector<char>& platform_username,
        const std::vector<char>& platform,
        bool                     special_chars_flag,
        bool                     digit_flag,
        bool                     capital_flag,
        bool                     non_capital_flag,
        int                      len);

    /**
     * @param username Username of the user who created this request.
     */
    static IRequest* create_password_get_request(
        const std::vector<char>& username);

    /**
     * @param username Username of the user who created this request.
     * @param pass Vector representing the generated password to be
     * deleted.
     */
    static IRequest* create_password_del_request(
        const std::vector<char>& username, const std::vector<char>& pass);

    /**
     * @param username Username of the user creating this request.
     * @param email Email of the username creating this request.
     */
    static IRequest* create_email_change_request(
        const std::vector<char>& user, const std::vector<char>& email);

    /**
     * @param username Username of the user who created this request.
     * @param hash Hash of the new password.
     */
    static IRequest* create_pass_change_request(
        const std::vector<char>& user, const std::vector<char>& hash);

    /**
     * @param username Username of the user who created this request.
     */
    static IRequest* create_recovery_code_request(
        const std::vector<char>& user);

    /**
     * @param username Username of the user who created this request.
     * @param code The code the user entered to reset his password.
     */
    static IRequest* create_recovery_code_check_request(
        const std::vector<char>& user, const std::vector<char>& code);
};

#endif  // REQUESTFACTORY_HXX