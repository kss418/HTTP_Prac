#pragma once
#include <string>
#include <vector>
#include "FsHelper.hpp"

class CmdHelper{
public:
    CmdHelper();
    void get_cmd();
private:
    std::vector <std::string> parse_cmd(const std::string& input);
    void execute_cmd(const std::vector<std::string>& arg);
};