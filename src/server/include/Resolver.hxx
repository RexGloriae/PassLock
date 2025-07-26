#ifndef RESOLVER_HXX
#define RESOLVER_HXX

class LoginRequest;
class RegisterRequest;
class SaltFetchRequest;
class PasswordGenRequest;
class PasswordGetRequest;
class PasswordDelRequest;
class EmailChangeRequest;
class PasswordChangeRequest;
class RecoveryCodeFetchRequest;
class RecoveryCodeCheckRequest;

#include <vector>

class IResponse;

class Resolver {
   private:
    static Resolver* m_inst;

    Resolver() = default;
    ~Resolver() = default;

    /**
     * @brief Sets an vector of chars with the text "Collision".
     *
     * The vector is used for sending a response for an associated
     * username-platform collision to the client.
     *
     * @return A std::vector<char> having the value of "Collision".
     */
    std::vector<char> set_collision();

    /**
     * @brief Re-encrypts all data of an user with KEY & IV generated from
     * the new passowrd.
     *
     * @param user User's name.
     * @param new_pass Hash of the new password.
     */
    void change_pass(const std::vector<char>& user,
                     const std::vector<char>& new_pass);

   public:
    Resolver(const Resolver& other) = delete;
    Resolver(Resolver&& other) = delete;

    static void      init();
    static Resolver& get();
    static void      kill();

    /**
     * @brief Solves a login request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const LoginRequest& req);

    /**
     * @brief Solves a register request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const RegisterRequest& req);

    /**
     * @brief Solves a salt fetch request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const SaltFetchRequest& req);

    /**
     * @brief Solves a password generation request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const PasswordGenRequest& req);

    /**
     * @brief Solves a password-fetching request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const PasswordGetRequest& req);

    /**
     * @brief Solves a password deletion request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const PasswordDelRequest& req);

    /**
     * @brief Solves an email change request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const EmailChangeRequest& req);

    /**
     * @brief Solves a password change request.
     *
     * @return A pointer of IResponse type representing the response to the
     * solved request.
     */
    IResponse* solve(const PasswordChangeRequest& req);
    IResponse* solve(const RecoveryCodeFetchRequest& req);
    IResponse* solve(const RecoveryCodeCheckRequest& req);

    /**
     * @brief Randomly generates a 4 digit array used as a recovery code
     * for an user.
     *
     * @return A std::vector<char> of size 4, having a digit on each cell.
     */
    std::vector<char> gen_recovery_code();
};

#endif  // RESOLVER_HXX