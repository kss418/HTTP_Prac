add_executable(server
    src/DBHelper.cpp
    src/Session.cpp
    src/SessionHandler.cpp
    src/Acceptor.cpp
    src/Service.cpp
    main.cpp
)

target_link_libraries(server
    PRIVATE
    unofficial::mariadb-connector-cpp::mariadbcpp
    aws-cpp-sdk-core
    nlohmann_json::nlohmann_json
    Boost::beast
    Boost::asio
)

target_include_directories(server
    PRIVATE
    ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include
    ${PROJECT_SOURCE_DIR}/include
)