#pragma once
#include <expected>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace Utility{
    std::expected<json, std::string> parse_json(const std::string& body);
};