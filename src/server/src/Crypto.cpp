#include "../include/Crypto.hxx"

#include <iomanip>
#include <sstream>

#include "../include/Exception.hxx"
#include "../include/Server.hxx"

Crypto* Crypto::m_inst = nullptr;

void Crypto::init(void) {
    if (Crypto::m_inst == nullptr) {
        Crypto::m_inst = new Crypto;
    }
}
void Crypto::kill(void) {
    if (Crypto::m_inst != nullptr) {
        delete Crypto::m_inst;
        Crypto::m_inst = nullptr;
    }
}
Crypto& Crypto::get(void) {
    if (Crypto::m_inst == nullptr) {
        throw Exception("Crypto class not initialized", 0x01);
    }
    return *(Crypto::m_inst);
}

std::string Crypto::sha3_512(const std::string& input) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();  // Create a new digest context
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    const EVP_MD* md =
        EVP_sha3_512();  // Select the SHA-512 hashing algorithm

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize digest");
    }

    if (EVP_DigestUpdate(ctx, input.data(), input.size()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update digest");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];  // Buffer for the hash output
    unsigned int  hashLen = 0;

    if (EVP_DigestFinal_ex(ctx, hash, &hashLen) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(ctx);  // Clean up

    // Convert the hash to a hex string
    std::ostringstream oss;
    for (size_t i = 0; i < hashLen; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<int>(hash[i]);
    }

    return oss.str();
}

std::vector<char> Crypto::aes_256_cbc_encr(
    const std::vector<char>& plain_text,
    const std::vector<char>& key,
    const std::vector<char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw Exception("Crypto err", 0x01);

    std::vector<char> ciphertext(plain_text.size() +
                                 Crypto::DEF::AES_BLOCK_SIZE);

    int len = 0, ciphertext_len = 0;

    // Initialize encryption operation
    if (EVP_EncryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            NULL,
            reinterpret_cast<const unsigned char*>(key.data()),
            reinterpret_cast<const unsigned char*>(iv.data())) != 1)
        throw Exception("Crypto err", 0x01);

    // Encrypt data
    if (EVP_EncryptUpdate(
            ctx,
            reinterpret_cast<unsigned char*>(ciphertext.data()),
            &len,
            reinterpret_cast<const unsigned char*>(plain_text.data()),
            plain_text.size()) != 1)
        throw Exception("Crypto err", 0x01);
    ciphertext_len = len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(
            ctx,
            reinterpret_cast<unsigned char*>(ciphertext.data()) + len,
            &len) != 1)
        throw Exception("Crypto err", 0x01);
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);  // Resize to actual data length
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

std::vector<char> Crypto::aes_256_cbc_decr(
    const std::vector<char>& cipher_text,
    const std::vector<char>& key,
    const std::vector<char>& iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw Exception("Crypto err", 0x01);

    std::vector<char> plaintext(cipher_text.size());
    int               len = 0, plaintext_len = 0;

    // Initialize decryption operation
    if (EVP_DecryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            NULL,
            reinterpret_cast<const unsigned char*>(key.data()),
            reinterpret_cast<const unsigned char*>(iv.data())) != 1)
        throw Exception("Crypto err", 0x01);

    // Decrypt data
    if (EVP_DecryptUpdate(
            ctx,
            reinterpret_cast<unsigned char*>(plaintext.data()),
            &len,
            reinterpret_cast<const unsigned char*>(cipher_text.data()),
            cipher_text.size()) != 1)
        throw Exception("Crypto err", 0x01);
    plaintext_len = len;

    // Finalize decryption
    if (EVP_DecryptFinal_ex(
            ctx,
            reinterpret_cast<unsigned char*>(plaintext.data()) + len,
            &len) != 1)
        throw Exception("Crypto err", 0x01);
    plaintext_len += len;

    plaintext.resize(plaintext_len);  // Resize to actual data length
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

std::vector<char> Crypto::aes_256_cbc_encr(
    const std::vector<char>& plain_text) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw Exception("Crypto err", 0x01);

    std::vector<char> ciphertext(plain_text.size() +
                                 Crypto::DEF::AES_BLOCK_SIZE);

    int len = 0, ciphertext_len = 0;

    // Initialize encryption operation
    if (EVP_EncryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            NULL,
            reinterpret_cast<const unsigned char*>(m_server_key.data()),
            reinterpret_cast<const unsigned char*>(m_server_iv.data())) !=
        1)
        throw Exception("Crypto err", 0x01);

    // Encrypt data
    if (EVP_EncryptUpdate(
            ctx,
            reinterpret_cast<unsigned char*>(ciphertext.data()),
            &len,
            reinterpret_cast<const unsigned char*>(plain_text.data()),
            plain_text.size()) != 1)
        throw Exception("Crypto err", 0x01);
    ciphertext_len = len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(
            ctx,
            reinterpret_cast<unsigned char*>(ciphertext.data()) + len,
            &len) != 1)
        throw Exception("Crypto err", 0x01);
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);  // Resize to actual data length
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext;
}

std::vector<char> Crypto::aes_256_cbc_decr(
    const std::vector<char>& cipher_text) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw Exception("Crypto err", 0x01);

    std::vector<char> plaintext(cipher_text.size());
    int               len = 0, plaintext_len = 0;

    // Initialize decryption operation
    if (EVP_DecryptInit_ex(
            ctx,
            EVP_aes_256_cbc(),
            NULL,
            reinterpret_cast<const unsigned char*>(m_server_key.data()),
            reinterpret_cast<const unsigned char*>(m_server_iv.data())) !=
        1)
        throw Exception("Crypto err", 0x01);

    // Decrypt data
    if (EVP_DecryptUpdate(
            ctx,
            reinterpret_cast<unsigned char*>(plaintext.data()),
            &len,
            reinterpret_cast<const unsigned char*>(cipher_text.data()),
            cipher_text.size()) != 1)
        throw Exception("Crypto err", 0x01);
    plaintext_len = len;

    // Finalize decryption
    if (EVP_DecryptFinal_ex(
            ctx,
            reinterpret_cast<unsigned char*>(plaintext.data()) + len,
            &len) != 1)
        throw Exception("Crypto err", 0x01);
    plaintext_len += len;

    plaintext.resize(plaintext_len);  // Resize to actual data length
    EVP_CIPHER_CTX_free(ctx);
    return plaintext;
}

void Crypto::gen_pbkdf2(const std::string&       pass,
                        const std::vector<char>& salt,
                        int                      salt_len,
                        int                      iter,
                        std::vector<char>&       key) {
    key.resize(64);
    int res = PKCS5_PBKDF2_HMAC(
        pass.c_str(),
        pass.length(),
        reinterpret_cast<const unsigned char*>(salt.data()),
        salt.size(),
        iter,
        EVP_sha256(),
        64,
        reinterpret_cast<unsigned char*>(key.data()));
}

std::vector<char> Crypto::gen_aes_key(const std::vector<char>& pbkdf2) {
    std::vector<char> key = pbkdf2;
    int middle = key.size() / 2;
    for (int i = 0; i < middle; i++) {
        key[i] ^= key[middle + i];
    }
    key.resize(Crypto::DEF::AES_KEY_SIZE);

    return key;
}
std::vector<char> Crypto::gen_aes_iv(const std::vector<char>& aes_key) {
    std::vector<char> iv = aes_key;
    int middle = iv.size() / 2;
    for (int i = 0; i < middle; i++) {
        iv[i] ^= iv[middle + i];
    }
    iv.resize(Crypto::DEF::AES_IV_SIZE);

    return iv;
}

std::vector<char> Crypto::generate_random_array() {
    char size;
    RAND_bytes(reinterpret_cast<unsigned char*>(&size), 1);
    size &= 0x7F;
    std::vector<char> random(size);
    RAND_bytes(reinterpret_cast<unsigned char*>(random.data()), size);
    for (int i = 0; i < random.size(); i++) {
        random[i] &= 0x7f;
    }
    return random;
}

int Crypto::random_int(int beg, int end) {
    int r_int;
    RAND_bytes(reinterpret_cast<unsigned char*>(&r_int), 4);
    r_int &= 0x7FFFFFFF;
    r_int = r_int % (end - beg + 1) + beg;
    return r_int;
}
int Crypto::random_int() {
    int r_int;
    RAND_bytes(reinterpret_cast<unsigned char*>(&r_int), 4);
    r_int &= 0x7FFFFFFF;
    return r_int;
}

void Crypto::load_module() {
    m_server_salt = Server::get_salt();
    std::vector<char> pbkdf2_key;
    this->gen_pbkdf2(Server::get().get_hash(),
                     m_server_salt,
                     m_server_salt.size(),
                     m_server_salt.size(),
                     pbkdf2_key);
    m_server_key = this->gen_aes_key(pbkdf2_key);
    m_server_iv = this->gen_aes_iv(m_server_key);
}