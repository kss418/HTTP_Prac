#pragma once
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace Service{
    using json = nlohmann::json;
    bool sign_in(json body);
    int8_t sign_up(json body);
};