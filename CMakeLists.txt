cmake_minimum_required(VERSION 3.10)
project(ECIESProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 寻找 OpenSSL
find_package(OpenSSL REQUIRED)

# 包含目录
include_directories(include ${OPENSSL_INCLUDE_DIR})

# 添加源文件及头文件
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")

# 添加可执行目标
add_executable(ecies_program ${SOURCES})

# 链接 OpenSSL 库
target_link_libraries(ecies_program ${OPENSSL_LIBRARIES})

# 测试
enable_testing()
file(GLOB TESTS "tests/*.cpp")
foreach(test_file ${TESTS})
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file} ${SOURCES})
    target_link_libraries(${test_name} ${OPENSSL_LIBRARIES})
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()

# 文档生成（如果需要，可以使用 Doxygen）
# find_package(Doxygen)
# if (DOXYGEN_FOUND)
#    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
#    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#    add_custom_target(doc ALL ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} DEPENDS ${SOURCES} ${HEADERS})
# endif()
