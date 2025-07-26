#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <string>
#include <vector>

class Crypto {
public:
    enum DEF { AES_KEY_SIZE = 32, AES_IV_SIZE = 16, AES_BLOCK_SIZE = 16 };

private:
    // user hash (user for keys)
    std::string m_pass_hash;
    std::vector<char> m_salt;

    std::vector<char> m_client_key;
    std::vector<char> m_client_iv;

    static Crypto* m_inst;
    Crypto() = default;
    ~Crypto() = default;

    void set_client_key();
    void set_client_iv();

public:
    Crypto(const Crypto& other) = delete;
    Crypto(Crypto&& other) = delete;

    static void    init(void);
    static void    kill(void);
    static Crypto& get(void);

    std::string       sha3_512(const std::string& input);
    std::vector<char> aes_256_cbc_encr(const std::vector<char>& plain_text,
                                       const std::vector<char>& key,
                                       const std::vector<char>& iv);
    std::vector<char> aes_256_cbc_decr(
        const std::vector<char>& cipher_text,
        const std::vector<char>& key,
        const std::vector<char>& iv);
    std::vector<char> aes_256_cbc_encr(const std::vector<char>& plain_text);
    std::vector<char> aes_256_cbc_decr(const std::vector<char>& cipher_text);

    void gen_pbkdf2(const std::string&       pass,
                    const std::vector<char>& salt,
                    int                      salt_len,
                    int                      iter,
                    std::vector<char>&       key);

    std::vector<char> gen_aes_key(const std::vector<char>& pbkdf2);
    std::vector<char> gen_aes_iv(const std::vector<char>& aes_key);

    std::vector<char> generate_random_array();
    int               random_int(int beg, int end);
    int               random_int();

    void set_user_hash(const std::string& hash);
    void set_user_salt(const std::vector<char>& salt);
};

#endif  // CRYPTO_H
