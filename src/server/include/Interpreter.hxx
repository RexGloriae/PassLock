#ifndef INTERPRETER_HXX
#define INTERPRETER_HXX

#include "../include/Payload.h"

class IRequest;

class Interpreter {
   private:
    static Interpreter* m_inst;

    Interpreter() = default;
    ~Interpreter() = default;

    /**
     * @brief Interprets a login request payload.
     *
     * @param payload Raw binary data representing the login request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x01                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(hash_lengh)            - Length of the hashed password
     *
     * 0x(hash)                  - Hashed password (raw bytes)
     */
    IRequest* interpret_login_payload(const char* payload, int len);

    /**
     * @brief Interprets a register request payload.
     *
     * @param payload Raw binary data representing the register request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x02                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(hash_lengh)            - Length of the hashed password
     *
     * 0x(hash)                  - Hashed password (raw bytes)
     *
     * 0x(salt_size)             - Size of the salt array
     *
     * 0x(salt)                  - Salt (raw bytes)
     *
     * 0x(email_length)          - Length of user's email
     *
     * 0x(email)                 - Email (raw bytes)
     */
    IRequest* interpret_register_payload(const char* payload, int len);

    /**
     * @brief Interprets a salt request payload.
     *
     * @param payload Raw binary data representing the salt request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x03                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     */
    IRequest* interpret_salt_request_payload(const char* payload, int len);

    /**
     * @brief Interprets a password generation request payload.
     *
     * @param payload Raw binary data representing the password generation
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x04                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(assoc_username_len)    - Length of username associated with the
     * password
     *
     * 0x(assoc_username)        - Username associated with the password
     * (raw bytes)
     *
     * 0x(gen_pass_len)          - Length wanted for the generated
     * password.
     *
     * 0x(platform_len)          - Length of the associated platform
     *
     * 0x(platform)              - Associated platform (raw bytes)
     *
     * 0x(special_flag)          - Flag for special characters, true if
     * used, false otherwise
     *
     * 0x(digit_flag)            - Flag for digits, true if used, false
     * otherwise
     *
     * 0x(upper_case_flag)       - Flag for upper-case characters, true if
     * used, false otherwise
     *
     * 0x(lower_case_flag)       - Flag for lower-case characters, true if
     * used, false otherwise
     */
    IRequest* interpret_pass_gen_payload(const char* payload, int len);

    /**
     * @brief Interprets a stored passwords request payload.
     *
     * @param payload Raw binary data representing the stored passwords
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x07                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     */
    IRequest* interpret_pass_request_payload(const char* payload, int len);

    /**
     * @brief Interprets a password delete request payload.
     *
     * @param payload Raw binary data representing the password delete
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x08                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(pass_len)              - Length of password to be deleted
     *
     * 0x(pass)                  - Password to be deleted (raw bytes)
     */
    IRequest* interpret_pass_del_payload(const char* payload, int len);

    /**
     * @brief Interprets an email change request payload.
     *
     * @param payload Raw binary data representing the email change
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x09                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     */
    IRequest* interpret_email_change_payload(const char* payload, int len);

    /**
     * @brief Interprets a password change request payload.
     *
     * @param payload Raw binary data representing the password change
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x0A                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(hash_size)             - Size of the new password hash
     *
     * 0x(hash)                  - New password hash (raw bytes)
     */
    IRequest* interpret_pass_change_payload(const char* payload, int len);

    /**
     * @brief Interprets a recovery code request payload.
     *
     * @param payload Raw binary data representing the recovery code
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x0B                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     */
    IRequest* interpret_recovery_code_request_payload(const char* payload,
                                                      int         len);

    /**
     * @brief Interprets a recovery code check request payload.
     *
     * @param payload Raw binary data representing the recovery code check
     * request.
     * @param len Payload length.
     *
     * @return An IRequest pointer representing the interpreted request.
     *
     *
     * Payload structure:
     * 0x0C                      - Type Byte
     *
     * 0x(username_length)       - Length of the username
     *
     * 0x(username)              - Username (raw bytes)
     *
     * 0x(code_size)             - Size of the recovery code
     *
     * 0x(code)                  - The recovery code to check (raw bytes)
     */
    IRequest* interpret_recovery_code_check_payload(const char* payload,
                                                    int         len);

   public:
    Interpreter(const Interpreter& other) = delete;
    Interpreter(Interpreter&& other) = delete;

    static void         init();
    static Interpreter& get();
    static void         kill();

    /**
     * @brief Interprets a payload and returns a specific type of Request.
     *
     * @param payload String of binary data.
     * @param len Payload length.
     *
     * @return Interpreted request.
     */
    IRequest* interpret_and_fetch(const char* payload, int len);
};

#endif

/*
payload structures:
login request=
0x01 0x(username_len) 0x(username) 0x(hash len) 0x(hash)

register request =
0x02 0x(username_len) 0x(username) 0x(hash len) 0x(hash) 0x(salt size)
0x(salt) 0xemail_size 0xemail

salt request
0x03 0x(username_len) 0x(username)

password generate request
0x04 0x(username_len) 0x(username) 0x(pass_user_len) 0xpass_username
0xpass_len 0xplatform_len 0xplatform 0xspecial_flag 0xdigit_flag
0xcapital_flag 0xnon_cap_flag

stored passwords request
0x07 0xusername_len 0xusername

email change
0x09 0xusername_len 0xusername 0xemail_size 0xemail

pass change
0x10 0xusername_len 0xusername 0xhass_size 0xhash
*/