#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    bool login(json body);
};