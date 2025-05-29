#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    void Service::sign_in(const json& json);
};