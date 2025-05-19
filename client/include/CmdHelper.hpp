#pragma once
#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "FsHelper.hpp"

class CmdHelper{
public:
    void get_cmd();
    CmdHelper(boost::asio::io_context& io_context);
private:
    std::vector <std::string> parse_cmd(const std::string& input);
    void execute_cmd(const std::vector<std::string>& arg);
    void cd(const std::vector<std::string>& arg);
    void mkdir(const std::vector<std::string>& arg);
    void rmdir(const std::vector<std::string>& arg);
    void ls();
    bool sign_in(const std::vector<std::string>& arg);
    int8_t sign_up(const std::vector<std::string>& arg);

    boost::asio::io_context& m_io_context;
};