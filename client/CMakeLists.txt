add_executable(client
    main.cpp
)

target_link_libraries(client
    PRIVATE
    unofficial::mariadb-connector-cpp::mariadbcpp
    aws-cpp-sdk-core
    nlohmann_json::nlohmann_json
    Boost::beast
    Boost::asio
)

target_include_directories(client
    PRIVATE
    ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include
)
include_directories(${PROJECT_SOURCE_DIR}/include)