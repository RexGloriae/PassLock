#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <sqlite3.h>

#include <string>
#include <vector>

class Database {
   private:
    std::string m_path = "../db/";

    static Database* m_inst;

    sqlite3* m_db;

    Database() = default;
    ~Database() = default;

    std::string create_path(std::string file);
    void        executeQuery(const std::string& query);
    void        createTables();

   public:
    Database(const Database& other) = delete;
    Database(Database&& other) = delete;

    static void      init(std::string name);
    static Database& get();
    static void      kill();

    /**
     * @brief Adds a new user in the database.
     *
     * @param username User's name.
     * @param pass_hash User's hashed password.
     * @param salt User's salt used for PBKDF2 generation.
     * @param email User's email.
     * @param recovery User's 4 digit recovery code.
     */
    void add_new_user(const std::vector<char>& username,
                      const std::vector<char>& pass_hash,
                      const std::vector<char>& salt,
                      const std::vector<char>& email,
                      const std::vector<char>& recovery);

    /**
     * @brief Fetches the hashed password of an user.
     *
     * @param username User's name.
     *
     * @return User's hashed password.
     */
    std::vector<char> get_password_hash(const std::vector<char>& username);

    /**
     * @brief Fetches the salt for an user.
     *
     * @param username User's name.
     *
     * @return User's salt.
     */
    std::vector<char> get_user_salt(const std::vector<char>& username);

    /**
     * @brief Fetches the email of an user.
     *
     * @param username User's name.
     *
     * @return User's email.
     */
    std::vector<char> get_user_email(const std::vector<char>& username);

    /**
     * @brief Fetches the recovery code of an user.
     *
     * @param username User's name.
     *
     * @return User's recovery code.
     */
    std::vector<char> get_user_recovery_code(
        const std::vector<char>& username);

    /**
     * @brief Updates the recovery code of an user.
     *
     * @param username User's name.
     * @param code New code to be added.
     */
    void add_recovery_code(const std::vector<char>& username,
                           const std::vector<char>& code);

    /**
     * @brief Inserts a new generated password for an user.
     *
     * @param username User's name.
     * @param associated_username Meta-data: Username associated to the
     * generated password.
     * @param platform Meta-data: Platform associated to the generated
     * password.
     * @param pass Generated password.
     */
    void insert_new_password(const std::vector<char>& username,
                             const std::vector<char>& associated_username,
                             const std::vector<char>& platform,
                             const std::vector<char>& pass);

    /**
     * @brief Deletes an user.
     *
     * @param username User's name.
     */
    void delete_user(const std::vector<char>& username);

    void delete_password(const std::vector<char>& username,
                         const std::vector<char>& pass);

    /**
     * @brief Retrieves all stored passwords for a specific user.
     *
     * @param username The username whose passwords are being requested.
     *
     * @return A vector of tuples, where each tuple contains:
     *
     *         - std::vector<char>: The platform where the password is
     * used.
     *
     *         - std::vector<char>: The password itself.
     *
     *         - std::vector<char>: The username associated with that
     * password on the platform.
     */
    std::vector<std::tuple<std::vector<char>,
                           std::vector<char>,
                           std::vector<char>>>
    get_all_passwords_for_user(const std::vector<char>& username);

    /**
     * @brief Checks for collisions of generated passwords that have
     * the same associated platform and username.
     *
     * @param username User's name.
     * @param platform Platform associated to the generated password.
     * @param associated_username Username associated to the generated
     * passwords.
     *
     * @return True if a collision is found, false otherwise.
     */
    bool is_collision(const std::vector<char>& username,
                      const std::vector<char>& platform,
                      const std::vector<char>& associated_username);

    /**
     * @brief Changes the email of an user.
     *
     * @param username User's name.
     * @param new_email New email to be added.
     */
    void update_user_email(const std::vector<char>& username,
                           const std::vector<char>& new_email);
};

#endif  // DATABASE_HXX