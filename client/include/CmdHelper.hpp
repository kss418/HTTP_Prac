#pragma once
#include <string>
#include <vector>
#include "FsHelper.hpp"

class CmdHelper{
public:
    void get_cmd();

    CmdHelper() = default;
private:
    std::vector <std::string> parse_cmd(const std::string& input);
    void execute_cmd(const std::vector<std::string>& arg);
    void cd(const std::vector<std::string>& arg);
    void mkdir(const std::vector<std::string>& arg);
    void rmdir(const std::vector<std::string>& arg);
    void ls();
};