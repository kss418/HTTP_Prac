#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    bool sign_in(json body);
    bool sign_up(json body);
};