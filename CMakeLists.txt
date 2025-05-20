cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0028 NEW)
project(HTTP_Prac)
set(CMAKE_PREFIX_PATH "/home/kss418/.vscode/HTTP_Prac/vcpkg/installed/x64-linux/share" CACHE STRING "")
set(CMAKE_TOOLCHAIN_FILE "/home/kss418/.vscode/HTTP_Prac/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(unofficial-mariadb-connector-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS beast asio)
find_package(OpenSSL REQUIRED)

add_subdirectory(client)
add_subdirectory(server)
