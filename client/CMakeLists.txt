add_executable(client
    src/Utility.cpp
    src/CmdHelper/CmdHelper.cpp
    src/CmdHelper/ClientCmdList.cpp
    src/CmdHelper/LoginCmdList.cpp
    src/CmdHelper/ServerCmdList.cpp
    src/CmdHelper/CmdExecuter.cpp
    src/FsHelper.cpp
    src/ServerFsHelper.cpp
    src/Session.cpp
    src/SessionHandler.cpp
    src/Service.cpp
    main.cpp
)

target_include_directories(client
    PRIVATE
    ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/include
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(client
    PRIVATE
    nlohmann_json::nlohmann_json
    Boost::beast
    Boost::asio
)