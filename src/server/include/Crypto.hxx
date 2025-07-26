#ifndef CRYPTO_HXX
#define CRYPTO_HXX

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <string>
#include <vector>

class Crypto {
   public:
    enum DEF { AES_KEY_SIZE = 32, AES_IV_SIZE = 16, AES_BLOCK_SIZE = 16 };

   private:
    std::vector<char> m_server_salt;
    std::vector<char> m_server_key;
    std::vector<char> m_server_iv;

    static Crypto* m_inst;
    Crypto() = default;
    ~Crypto() = default;

   public:
    Crypto(const Crypto& other) = delete;
    Crypto(Crypto&& other) = delete;

    static void    init(void);
    static void    kill(void);
    static Crypto& get(void);

    /**
     * @brief Hashes an input using the SHA3-512 algorithm.
     *
     * @param input input to be hashed.
     *
     * @return Hashed input.
     */
    std::string sha3_512(const std::string& input);

    /**
     * @brief Encrypts a plain text using the AES 256 CBC algorithm.
     *
     * @param plain_text Input to be encrypted.
     * @param key Key used for encryption.
     * @param iv Initializing Vector.
     *
     * @return Encrypted output.
     */
    std::vector<char> aes_256_cbc_encr(const std::vector<char>& plain_text,
                                       const std::vector<char>& key,
                                       const std::vector<char>& iv);

    /**
     * @brief Decrypts a cypher text encrypted with the AES 256 CBC
     * algorithm
     *
     * @param cipher_text Encrypted input.
     * @param key Key used for decryption
     * @param iv Initializing Vector.
     *
     * @return Decrypted output.
     */
    std::vector<char> aes_256_cbc_decr(
        const std::vector<char>& cipher_text,
        const std::vector<char>& key,
        const std::vector<char>& iv);

    /**
     * @brief Encrypts a plain text using the AES 256 CBC Algorithm.
     * Encryption Key and Initializing Vector are the ones used for Server
     * Side Encryption.
     *
     * @param plain_text Input to be encrypted.
     *
     * @return Encrypted Output.
     */
    std::vector<char> aes_256_cbc_encr(
        const std::vector<char>& plain_text);

    /**
     * @brief Decrypts data using the AES 256 CBC Algorithm.
     * Decryption Key and Initializing Vector are the ones used for Server
     * Side Encryption.
     *
     * @param cipher_text Input to be decrypted.
     *
     * @return Decrypted Output.
     */
    std::vector<char> aes_256_cbc_decr(
        const std::vector<char>& cipher_text);

    /**
     * @brief Generates a PBKDF2 Key.
     *
     * @param pass Input password used for generation.
     * @param salt Salt array used for generation.
     * @param salt_len Length of salt array.
     * @param iter Number of iterations to be done.
     * @param[out] key Returns the generated PBKDF2 key.
     *
     */
    void gen_pbkdf2(const std::string&       pass,
                    const std::vector<char>& salt,
                    int                      salt_len,
                    int                      iter,
                    std::vector<char>&       key);

    /**
     * @brief Generates an AES Encryption key based on a PBKDF2 Key.
     *
     * @param pbkdf2 Value of the PBKDF2 key.
     *
     * @return AES Encryption Key.
     */
    std::vector<char> gen_aes_key(const std::vector<char>& pbkdf2);

    /**
     * @brief Generates an AES Initializing Vector based on an Encryption
     * key.
     *
     * @param aes_key Value of the AES Encryption key.
     *
     * @return AES Initializing Vector.
     */
    std::vector<char> gen_aes_iv(const std::vector<char>& aes_key);

    /**
     * @brief Generates a random array of random size.
     *
     * @return The random array.
     */
    std::vector<char> generate_random_array();

    /**
     * @brief Generates a random integer value.
     *
     * @param beg Minimum value of the random integer.
     * @param end Maximum value of the random integer.
     *
     * @return Random integer value.
     */
    int random_int(int beg, int end);

    /**
     * @brief Generates a random integer value.
     *
     * @return Random integer value.
     */
    int random_int();

    /**
     * @brief Loads the Cryptographic module by fetching the KEY and IV
     * used for Server Side Encryption.
     */
    void load_module();
};

#endif  // CRYPTO_HXX