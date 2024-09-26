# ECIESManager

`ECIESManager` 是一个基于 ECIES (Elliptic Curve Integrated Encryption Scheme) 的加密管理类，提供 ECC 密钥生成、消息加密和解密的功能。

## 功能

- 生成 ECC 密钥对。
- 使用公钥加密消息。
- 使用私钥解密消息。
- 计算 HMAC 校验实现消息完整性。
- 从文件中读取消息。
- 记录日志信息。

## 依赖项

使用本类之前，请确保已安装以下依赖项：

- OpenSSL：用于加密和 HMAC 操作。
- C++ 标准库：用于数据结构和文件操作。

## 类说明

### ECIESManager

用于管理 ECC 加密操作的类。

#### 构造函数

```cpp
ECIESManager(const std::string &log_file_path);
```
- `log_file_path`：日志文件的路径，用于记录日志信息。

#### 成员函数

- **`EC_KEY* generate_ecc_key()`**
  - 生成一个新的 ECC 密钥。
  
- **`std::vector<unsigned char> encrypt(const std::string &message, EC_KEY *public_key)`**
  - 使用给定的公钥加密消息。
  
- **`std::string decrypt(const std::vector<unsigned char> &encrypted_message, EC_KEY *private_key)`**
  - 使用给定的私钥解密消息。
  
- **`std::string read_file(const std::string &filename)`**
  - 读取指定文件的内容。
  
- **`void log(const std::string &message)`**
  - 将日志信息写入指定的日志文件。
  
- **`std::vector<unsigned char> calculate_hmac(const unsigned char *data, size_t data_len, const std::vector<unsigned char> &key)`**
  - 根据给定的数据和密钥计算 HMAC。

## 使用示例

```cpp
#include "ECIESManager.h"

int main() {
    // 创建 ECIESManager 实例
    ECIESManager manager("log.txt");

    // 生成 ECC 密钥
    EC_KEY* key = manager.generate_ecc_key();

    // 待加密消息
    std::string message = "Hello, ECIES!";

    // 加密
    auto encrypted_message = manager.encrypt(message, EC_KEY_get0_public_key(key));

    // 解密
    std::string decrypted_message = manager.decrypt(encrypted_message, key);

    std::cout << "Decrypted Message: " << decrypted_message << std::endl;

    // 清理
    EC_KEY_free(key);
    return 0;
}
```

## 许可

本项目采用 MIT 许可协议。有关详细信息，请查看 [LICENSE](LICENSE) 文件。