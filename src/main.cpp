#include "ECIESManager.h"

/**

 * @brief 主要功能描述

 * 

 * @param argc 参数数量

 * @param argv 参数列表

 * @return int 返回值

 */
int main(int argc, char *argv[]) {
    // 默认文件路径
    std::string log_file_path = "ecies_log.txt";
    std::string input_file_path = "input.txt"; // 输入文件路径

    // 检查命令行参数，获取文件路径
    if (argc > 1) {
        log_file_path = argv[1];  // 第一个参数为日志文件路径
    }
    if (argc > 2) {
        input_file_path = argv[2]; // 第二个参数为输入文件路径
    }

    ECIESManager ecies_manager(log_file_path);

    try {
        // 生成 ECC 密钥对
        EC_KEY *ecc_private_key = ECIESManager::generate_ecc_key();
        EC_KEY *ecc_public_key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);
        EC_KEY_copy(ecc_public_key, ecc_private_key);

        // 从文件读取消息
        std::string message = ecies_manager.read_file(input_file_path);
        
        std::vector<unsigned char> encrypted_message = ecies_manager.encrypt(message, ecc_public_key);
        
        std::cout << "Encrypted Message: ";
        for (unsigned char c : encrypted_message) {
            std::cout << std::hex << (int)c << " ";
        }
        std::cout << std::dec << std::endl;

        std::string decrypted_message = ecies_manager.decrypt(encrypted_message, ecc_private_key);
        std::cout << "Decrypted Message: " << decrypted_message << std::endl;

        EC_KEY_free(ecc_private_key);
        EC_KEY_free(ecc_public_key);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
