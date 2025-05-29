#include "../include/Utility.hpp"

std::expected<json, std::string> Utility::parse_json(const std::string& body){
    if(!json::accept(body)){
        return std::unexpected("서버 응답 파싱 불가");
    }
    return json::parse(body);
}