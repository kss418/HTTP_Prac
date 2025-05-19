cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0028 NEW)
project(AWS_Prac)
set(CMAKE_PREFIX_PATH "/home/kss418/.vscode/AWS_Prac/vcpkg/installed/x64-linux/share" CACHE STRING "")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(unofficial-mariadb-connector-cpp CONFIG REQUIRED)
find_package(aws-cpp-sdk-core REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS beast asio)

add_subdirectory(client)
add_subdirectory(server)
