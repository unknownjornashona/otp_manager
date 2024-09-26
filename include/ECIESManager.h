#ifndef ECIES_MANAGER_H
#define ECIES_MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <openssl/ec.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/hmac.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

class ECIESManager {
public:
    ECIESManager(const std::string &log_file_path);
    static EC_KEY* generate_ecc_key();
    std::vector<unsigned char> encrypt(const std::string &message, EC_KEY *public_key);
    std::string decrypt(const std::vector<unsigned char> &encrypted_message, EC_KEY *private_key);
    std::string read_file(const std::string &filename);
    
private:
    std::string log_file_path;
    void log(const std::string &message);
    std::vector<unsigned char> calculate_hmac(const unsigned char *data, size_t data_len, const std::vector<unsigned char> &key);
};

#endif // ECIES_MANAGER_H
