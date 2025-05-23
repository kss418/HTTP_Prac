#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>

namespace Service{
    using json = nlohmann::json;

    bool sign_in(json json);
    bool sign_up(json json);
    bool mkdir(json json);
    std::string download(json json);
};