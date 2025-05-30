#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    void sign_in(const json& json, const std::string& id, bool& logged_in);
    void sign_up();
    void server_mkdir();
    void server_cd(const json& json);
    void server_ls(const json& json);
    void server_rmdir(const json& json);
    void server_rm(const json& json);
};