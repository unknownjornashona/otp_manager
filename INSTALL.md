# ECIESManager 安装指南

## 前提条件

在安装 `ECIESManager` 之前，请确保您具备以下环境：

- 操作系统：Linux、macOS 或 Windows
- C++ 编译器：支持 C++11 或更高版本（如 GCC、Clang、MSVC）
- CMake：用于构建项目

## 安装步骤

1. **克隆项目**

   打开终端并运行以下命令，将 `ECIESManager` 项目克隆到本地：

   ```bash
   git clone https://github.com/username/ECIESManager.git
   ```

   请将 `username` 替换为实际的 GitHub 用户名。

2. **进入项目目录**

   进入克隆下来的项目目录：

   ```bash
   cd ECIESManager
   ```

3. **创建构建目录**

   为了保持项目目录整洁，建议创建一个单独的构建目录：

   ```bash
   mkdir build
   cd build
   ```

4. **运行 CMake**

   使用 CMake 配置项目：

   ```bash
   cmake ..
   ```

5. **构建项目**

   使用以下命令编译项目：

   ```bash
   make
   ```

   如果您使用的是 Windows 和 Visual Studio，可以运行：

   ```bash
   cmake --build . --config Release
   ```

6. **运行示例**

   完成构建后，可以运行提供的示例程序（如有）：

   ```bash
   ./example  # 在 Linux 或 macOS
   example.exe  # 在 Windows
   ```

   示例程序将演示如何使用 `ECIESManager` 进行密钥生成、消息加密和解密。

## 其他信息

- **文档**：有关 `ECIESManager` 的详细信息、接口和用法，请查看 [DOCUMENTATION.md](DOCUMENTATION.md)。
- **支持**：如有问题，请在 [GitHub 问题页面](https://github.com/username/ECIESManager/issues) 提交问题或寻求支持。

```