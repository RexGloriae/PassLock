#ifndef PACKAGER_HXX
#define PACKAGER_HXX

#include <vector>

#include "../include/Payload.h"

class Packager {
   private:
    static Packager* m_inst;

    Packager() = default;
    ~Packager() = default;

   public:
    Packager(const Packager& other) = delete;
    Packager(Packager&& other) = delete;

    static void      init();
    static Packager& get();
    static void      kill();

    /**
     * @brief Encapsulate a login response.
     *
     * @param answ True if the login was done successfully, False
     * otherwise.
     */
    std::vector<char> encapsulate_login_response(bool answ);

    /**
     * @brief Encapsulate a register response.
     *
     * @param answ True if the login was done successfully, False
     * otherwise.
     */
    std::vector<char> encapsulate_register_response(bool answ);

    /**
     * @brief Encapsulate a Salt Fetch response.
     *
     * @param salt A vector representing the salt.
     */
    std::vector<char> encapsulate_salt_fetch_response(
        const std::vector<char>& salt);

    /**
     * @brief Encapsulate a password generation response.
     *
     * @param pass A vector representing the generated password.
     */
    std::vector<char> encapsulate_password_gen_response(
        const std::vector<char>& pass);

    /**
     * @brief Encapsulate a Password Fetch response.
     *
     * @param pass A vector of tuples, where each tuple contains:
     *
     *         - std::vector<char>: The platform where the password is
     * used.
     *
     *         - std::vector<char>: The password itself.
     *
     *         - std::vector<char>: The username associated with that
     * password on the platform.
     */
    std::vector<char> encapsulate_password_get_response(
        const std::tuple<std::vector<char>,
                         std::vector<char>,
                         std::vector<char>>& pass);

    /**
     * @brief Encapsulate a Email Change request.
     *
     * @param answ True if the email was changed successfully, False
     * otherwise.
     */
    std::vector<char> encapsulate_email_change_response(bool answ);

    /**
     * @brief Encapsulate a Recovery Code Fetch request.
     *
     * @param answ True if the recovery code was sent successfully, False
     * otherwise.
     */
    std::vector<char> encapsulate_recovery_code_fetch_response(bool answ);

    /**
     * @brief Encapsulate a Recovery Code Check request.
     *
     * @param answ True if the recovery code is equal to the stored one,
     * False otherwise.
     */
    std::vector<char> encapsulate_recovery_code_check_response(
        bool answer);
};

#endif  // PACKAGER_HXX

/*
payload structure:

login:
0x(code) 0x00/0x01 (false/true)

register:
0x(code) 0x00/0x01 (false/true)

salt fetch response:
0x(code) 0x(salt)

password generation response:
0x(code) 0xpass_len 0xpass

stored passwords response:
0x05 0xcount
i=0, count
0x06 0xpass_len 0xpass 0xplatform_size 0xplatform 0xusername_size
0xusername

*/