#ifndef RESPONSEFACTORY_HXX
#define RESPONSEFACTORY_HXX

#include "EmailChangeResponse.hxx"
#include "LoginResponse.hxx"
#include "PasswordGenResponse.hxx"
#include "PasswordGetResponse.hxx"
#include "RecoveryCodeCheckResponse.hxx"
#include "RecoveryCodeFetchResponse.hxx"
#include "RegisterResponse.hxx"
#include "SaltFetchResponse.hxx"

class ResponseFactory {
   public:
    /**
     * @param answ True if the login was done successfully, False
     * otherwise.
     */
    static IResponse* create_login_response(bool status);

    /**
     * @param answ True if the registration was done successfully, False
     * otherwise.
     */
    static IResponse* create_register_response(bool status);

    /**
     * @param salt Vector representing the salt used for client-side
     * encryption.
     */
    static IResponse* create_salt_fetch_response(
        const std::vector<char>& salt);

    /**
     * @param pass Vector representing the generated password.
     */
    static IResponse* create_pass_gen_response(
        const std::vector<char>& pass);

    /**
     * @param passwords A vector of tuples, where each tuple contains:
     *
     *         - std::vector<char>: The platform where the password is
     * used.
     *
     *         - std::vector<char>: The password itself.
     *
     *         - std::vector<char>: The username associated with that
     * password on the platform.
     */
    static IResponse* create_pass_get_response(
        const std::vector<std::tuple<std::vector<char>,
                                     std::vector<char>,
                                     std::vector<char>>>& passwords);

    /**
     * @param status True if the email was changed, False otherwise.
     */
    static IResponse* create_email_change_response(bool status);

    /**
     * @param status True if the recovery code was sent successfully, False
     * otherwise.
     */
    static IResponse* create_recovery_code_fetch_response(bool status);

    /**
     * @param status True if the recovery code is the right one, False
     * otherwise.
     */
    static IResponse* create_recovery_code_check_response(bool status);
};

#endif  // RESPONSEFACTORY_HXX