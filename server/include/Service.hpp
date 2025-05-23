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
    std::vector<std::pair<std::string, bool>> ls(json json);
    bool rm(json json);
    bool rmdir(json json);

    std::string cwd(json json);

    std::string download(json json);
};