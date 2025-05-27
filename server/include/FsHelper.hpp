#pragma once
#include <memory>
#include <mutex>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>

class FsExecuter{
public:
    FsExecuter(const std::string& id);
    std::filesystem::path cwd() const;
    bool exists(const std::filesystem::path& cwd) const;
    bool set_cwd(const std::filesystem::path& cwd);
    bool mkdir(const std::filesystem::path& cwd);
    int32_t rmdir(const std::filesystem::path& cwd);
    int32_t rm(const std::filesystem::path& cwd);
    nlohmann::json ls() const;
private:
    std::filesystem::path m_working_path;
};

class FsHelper{
public:
    static FsHelper& get_instance();
    std::unordered_map<std::string, std::unique_ptr<FsExecuter>> m_map;

    FsHelper() = default;
    FsHelper(const FsHelper&) = delete;
    FsHelper& operator=(const FsHelper&) = delete;
    FsHelper(FsHelper&&) = delete;
    FsHelper& operator=(FsHelper&&) = delete;
private:
    static std::once_flag m_init_flag;
    static std::unique_ptr<FsHelper> m_instance;
};
