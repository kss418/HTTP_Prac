#pragma once
#include <memory>
#include <mutex>
#include <filesystem>
#include <vector>
#include <boost/asio.hpp>

class ServerFsHelper{
public:
    static ServerFsHelper& get_instance();

    std::filesystem::path cwd() const;
    void set_cwd(const std::filesystem::path& cwd);
    void set_id(const std::string& id);

    void mkdir(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    void rmdir(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    void rm(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    void ls(boost::asio::io_context& io_context);
    
    void cd(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    void download(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    void upload(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    int32_t exist(
        const std::filesystem::path& path,
        boost::asio::io_context& io_context
    );

    ServerFsHelper() = default;
    ServerFsHelper(const ServerFsHelper&) = delete;
    ServerFsHelper& operator=(const ServerFsHelper&) = delete;
    ServerFsHelper(ServerFsHelper&&) = delete;
    ServerFsHelper& operator=(ServerFsHelper&&) = delete;
private:
    std::filesystem::path m_working_path;
    std::string m_id;
    static std::once_flag m_init_flag;
    static std::unique_ptr<ServerFsHelper> m_instance;
};