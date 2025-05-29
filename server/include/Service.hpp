#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <vector>

namespace Service{
    using json = nlohmann::json;

    bool sign_in(json json);
    bool sign_up(json json);

    bool mkdir(json json);
    bool cd(json json);
    json ls(const std::string& id);
    int32_t rm(json json);
    int32_t rmdir(json json);
    int32_t exist_file(
        const std::string& id, 
        const std::filesystem::path& path
    );
    void upload(
        const std::string& id, const std::string& path, 
        const std::string& file_name
    );

    std::string cwd(json json);
};
