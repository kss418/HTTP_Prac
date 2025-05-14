#pragma once
#include <memory>
#include <mutex>
#include <filesystem>

class FsHelper{
public:
    static FsHelper& get_instance();

    std::filesystem::path cwd() const;
    bool exists(const std::filesystem::path& cwd) const;
    void set_cwd(const std::filesystem::path& cwd);
    void mkdir(const std::filesystem::path& cwd);
    void rmdir(const std::filesystem::path& cwd);
    void ls() const;

    FsHelper() = default;
    FsHelper(const FsHelper&) = delete;
    FsHelper& operator=(const FsHelper&) = delete;
    FsHelper(FsHelper&&) = delete;
    FsHelper& operator=(FsHelper&&) = delete;
private:
    std::filesystem::path m_working_path;
    static std::once_flag m_init_flag;
    static std::unique_ptr<FsHelper> m_instance;
};