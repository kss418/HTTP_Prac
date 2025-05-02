cmake_minimum_required(VERSION 3.15)
project(AWS_Prac)
set(CMAKE_PREFIX_PATH "/home/kss418/.vscode/AWS_Prac/vcpkg/installed/x64-linux" CACHE STRING "")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(unofficial-mariadb-connector-cpp CONFIG REQUIRED)
find_package(aws-cpp-sdk-core REQUIRED)

add_executable(AWS_Prac
    main.cpp
)

target_link_libraries(AWS_Prac
    PRIVATE
    unofficial::mariadb-connector-cpp::mariadbcpp
    aws-cpp-sdk-core
)

target_include_directories(AWS_Prac
  PRIVATE
    ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include
)
include_directories(${PROJECT_SOURCE_DIR}/include)