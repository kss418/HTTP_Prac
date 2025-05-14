add_executable(client
    src/CmdHelper.cpp
    src/CmdList.cpp
    src/FsHelper.cpp
    src/Session.cpp
    src/SessionHandler.cpp
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
    ${PROJECT_SOURCE_DIR}/include
)