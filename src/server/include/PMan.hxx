#ifndef PMAN_HXX
#define PMAN_HXX

#include <string>
#include <vector>

class PMan {
   private:
    static PMan* m_inst;

    PMan() = default;
    ~PMan() = default;

    bool is_special(char ch) const;
    bool is_upper(char ch) const;
    bool is_lower(char ch) const;
    bool is_digit(char ch) const;

   public:
    PMan(const PMan& other) = delete;
    PMan(PMan&& other) = delete;

    static void  init();
    static void  kill();
    static PMan& get();

    /**
     * @brief Encrypts a password mimicking a client-side encryption.
     *
     * @param pass Password to be encrypted.
     * @param username User account who will use this password; necessary
     * for mimicking client-side encryption.
     *
     * @return Encrypted output.
     */
    std::vector<char> enc(std::vector<char> pass,
                          std::vector<char> username);

    /**
     * @brief Randomly generates a new password.
     *
     * @param username User account who will use this password; necessary
     * for mimicking client-side encryption.
     * @param len Wanted length for the generated password.
     * @param special_chars Flag for the usage of special characters in the
     * generated password; true if used, false otherwise.
     * @param digits Flag for the usage of digits in the generated
     * password; true if used, false otherwise
     * @param capital_letters Flag for the usage of upper-case letters in
     * the generated password; true if used, false otherwise.
     * @param non_capital_letters Flag for the usage of lower-case letters
     * in the generated password; true if used, false otherwise.
     *
     * @return A randomly generated password meeting all the
     * above-mentioned criteria.
     */
    std::vector<char> gen(std::vector<char> username,
                          int               len,
                          bool              special_chars,
                          bool              digits,
                          bool              capital_letters,
                          bool              non_capital_letters);
};

#endif  // PMAN_HXX