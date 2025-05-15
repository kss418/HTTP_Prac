#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    void login(json body);
};