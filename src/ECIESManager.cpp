#include "ECIESManager.h"

/**
 * @brief Construct a new ECIESManager object.
 * 
 * @param log_file_path The path to the log file for logging events.
 */
ECIESManager::ECIESManager(const std::string &log_file_path) : log_file_path(log_file_path) {}

/**
 * @brief Generate a new ECC key.
 * 
 * @return EC_KEY* Pointer to the generated EC key.
 * @throw std::runtime_error if key generation fails.
 */
EC_KEY* ECIESManager::generate_ecc_key() {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
    if (!key) throw std::runtime_error("Failed to create EC Key.");
    if (EC_KEY_generate_key(key) != 1) throw std::runtime_error("Key generation failed.");
    return key;
}

/**
 * @brief Encrypt a message using ECIES.
 * 
 * @param message The plaintext message to encrypt.
 * @param public_key The public key used for encryption.
 * @return std::vector<unsigned char> The encrypted message as a byte vector.
 * @throw std::runtime_error if shared secret computation or HMAC verification fails.
 */
std::vector<unsigned char> ECIESManager::encrypt(const std::string &message, EC_KEY *public_key) {
    // 生成共享密钥
    std::vector<unsigned char> shared_secret(ECDSA_size(public_key));
    int secret_len = ECDH_compute_key(shared_secret.data(), shared_secret.size(), EC_KEY_get0_public_key(public_key), EC_KEY_get0_private_key(public_key), nullptr);
    if (secret_len <= 0) throw std::runtime_error("Failed to compute shared secret.");

    std::vector<unsigned char> aes_key(shared_secret.begin(), shared_secret.begin() + 16); // AES-128
    std::vector<unsigned char> iv(16);
    RAND_bytes(iv.data(), iv.size());

    // AES 加密
    AES_KEY encrypt_key;
    AES_set_encrypt_key(aes_key.data(), 128, &encrypt_key);

    std::vector<unsigned char> encrypted_message(message.size() + AES_BLOCK_SIZE);
    AES_cbc_encrypt(reinterpret_cast<const unsigned char *>(message.data()), encrypted_message.data() + AES_BLOCK_SIZE, message.size(), &encrypt_key, iv.data(), AES_ENCRYPT);
    
    std::memcpy(encrypted_message.data(), iv.data(), iv.size());  // 将 IV 前置

    // HMAC 校验
    std::vector<unsigned char> hmac = calculate_hmac(encrypted_message.data(), encrypted_message.size(), aes_key);
    encrypted_message.insert(encrypted_message.end(), hmac.begin(), hmac.end());
    
    log("Message encrypted.");
    return encrypted_message;
}

/**
 * @brief Decrypt an encrypted ECIES message.
 * 
 * @param encrypted_message The byte vector containing the encrypted message.
 * @param private_key The private key used for decryption.
 * @return std::string The decrypted message as a string.
 * @throw std::runtime_error if HMAC verification fails or shared secret computation fails.
 */
std::string ECIESManager::decrypt(const std::vector<unsigned char> &encrypted_message, EC_KEY *private_key) {
    std::vector<unsigned char> iv(encrypted_message.begin(), encrypted_message.begin() + 16);
    const size_t hmac_size = 32; // 使用 SHA-256
    std::vector<unsigned char> hmac(encrypted_message.end() - hmac_size, encrypted_message.end());
    std::vector<unsigned char> ciphertext(encrypted_message.begin() + 16, encrypted_message.end() - hmac_size);

    // 验证 HMAC
    std::vector<unsigned char> shared_secret(ECDSA_size(private_key));
    int secret_len = ECDH_compute_key(shared_secret.data(), shared_secret.size(), EC_KEY_get0_public_key(private_key), EC_KEY_get0_private_key(private_key), nullptr);
    if (secret_len <= 0) throw std::runtime_error("Failed to compute shared secret.");

    std::vector<unsigned char> aes_key(shared_secret.begin(), shared_secret.begin() + 16); // AES-128
    std::vector<unsigned char> calculated_hmac = calculate_hmac(ciphertext.data(), ciphertext.size(), aes_key);
    if (hmac != calculated_hmac) {
        throw std::runtime_error("HMAC verification failed.");
    }

    // AES 解密
    AES_KEY decrypt_key;
    AES_set_decrypt_key(aes_key.data(), 128, &decrypt_key);

    std::vector<unsigned char> decrypted_message(ciphertext.size());
    AES_cbc_encrypt(ciphertext.data(), decrypted_message.data(), ciphertext.size(), &decrypt_key, iv.data(), AES_DECRYPT);

    log("Message decrypted.");
    return std::string(decrypted_message.begin(), decrypted_message.end());
}

/**
 * @brief Read the contents of a file into a string.
 * 
 * @param filename The path to the file to read.
 * @return std::string The contents of the file.
 * @throw std::runtime_error if the file cannot be opened.
 */
std::string ECIESManager::read_file(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading.");
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Log a message to the specified log file.
 * 
 * @param message The message to log.
 */
void ECIESManager::log(const std::string &message) {
    std::ofstream log_file(log_file_path, std::ios::app);
    if (log_file.is_open()) {
        log_file << message << std::endl;
        log_file.close();
    }
}

/**
 * @brief Calculate HMAC of the given data using the specified key.
 * 
 * @param data Pointer to the data buffer.
 * @param data_len Length of the data buffer.
 * @param key Key used for HMAC calculation.
 * @return std::vector<unsigned char> The calculated HMAC.
 */
std::vector<unsigned char> ECIESManager::calculate_hmac(const unsigned char *data, size_t data_len, const std::vector<unsigned char> &key) {
    std::vector<unsigned char> hmac(EVP_MAX_MD_SIZE);
    unsigned int hmac_len;
    HMAC(EVP_sha256(), key.data(), key.size(), data, data_len, hmac.data(), &hmac_len);
    hmac.resize(hmac_len);
    return hmac;
}
