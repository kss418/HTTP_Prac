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
    void login_execute(const std::vector<std::string>& arg);
    void logout_execute(const std::vector<std::string>& arg);
    void local_execute(const std::vector<std::string>& arg);
    void server_execute(const std::vector<std::string>& arg);

    void cd(const std::vector<std::string>& arg);
    void mkdir(const std::vector<std::string>& arg);
    void rmdir(const std::vector<std::string>& arg);
    void ls();

    void server_cd(const std::vector<std::string>& arg);
    void server_mkdir(const std::vector<std::string>& arg);
    void server_rmdir(const std::vector<std::string>& arg);
    void server_ls();

    void sign_in(const std::vector<std::string>& arg);
    void sign_up(const std::vector<std::string>& arg);
    void logout();

    void upload();
    void download();

    bool logged_in = 0;
    int8_t file_state = 0;
    int8_t load_state = 0;
    boost::asio::io_context& m_io_context;
};