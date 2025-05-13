#pragma once
#include <string>
#include <vector>

class CmdParser{
public:
    CmdParser();
    std::vector<std::string> get_parsed_cmd();
private:
    std::vector <std::string> parse_cmd(const std::string& input);
};