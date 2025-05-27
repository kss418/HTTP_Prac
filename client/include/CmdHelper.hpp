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
    void rm(const std::vector<std::string>& arg);
    void ls();
    void set_upload_file(const std::vector<std::string>& arg);
    void set_download_dir(const std::vector<std::string>& arg);

    void server_cd(const std::vector<std::string>& arg);
    void server_mkdir(const std::vector<std::string>& arg);
    void server_rmdir(const std::vector<std::string>& arg);
    void server_ls();
    void set_download_file(const std::vector<std::string>& arg);
    void set_upload_dir(const std::vector<std::string>& arg);

    void sign_in(const std::vector<std::string>& arg);
    void sign_up(const std::vector<std::string>& arg);
    void logout();

    void upload();
    void download();

    void change();

    bool logged_in = 0;
    bool path_state = 0;

    std::string id;
    std::string local_path;
    std::string server_path;
    boost::asio::io_context& m_io_context;
};