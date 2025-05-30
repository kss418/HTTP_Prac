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
    void set_cwd(const std::filesystem::path& dir_path);
    bool mkdir(const std::filesystem::path& any_path);
    int32_t rmdir(const std::filesystem::path& any_path);
    int32_t rm(const std::filesystem::path& any_path);
    nlohmann::json ls() const;
    
private:
    std::filesystem::path m_working_path;
};

class FsHelper{
public:
    static FsHelper& get_instance();
    std::unordered_map<std::string, std::unique_ptr<FsExecuter>> m_map;
    FsExecuter& get_executer(const std::string& id);

    FsHelper() = default;
    FsHelper(const FsHelper&) = delete;
    FsHelper& operator=(const FsHelper&) = delete;
    FsHelper(FsHelper&&) = delete;
    FsHelper& operator=(FsHelper&&) = delete;
private:
    static std::once_flag m_init_flag;
    static std::unique_ptr<FsHelper> m_instance;
};
