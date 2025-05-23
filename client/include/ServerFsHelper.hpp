#pragma once
#include <memory>
#include <mutex>
#include <filesystem>
#include <boost/asio.hpp>

class ServerFsHelper{
public:
    static ServerFsHelper& get_instance();

    std::filesystem::path cwd() const;
    void set_cwd(const std::filesystem::path& cwd);
    void mkdir(
        const std::filesystem::path& path, const std::string& id,
        boost::asio::io_context& io_context
    );
    void rmdir(const std::filesystem::path& cwd);
    void ls() const;

    ServerFsHelper() = default;
    ServerFsHelper(const ServerFsHelper&) = delete;
    ServerFsHelper& operator=(const ServerFsHelper&) = delete;
    ServerFsHelper(ServerFsHelper&&) = delete;
    ServerFsHelper& operator=(ServerFsHelper&&) = delete;
private:
    std::filesystem::path m_working_path;
    static std::once_flag m_init_flag;
    static std::unique_ptr<ServerFsHelper> m_instance;
};