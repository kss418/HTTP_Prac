#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"
#include "../../include/ServerFsHelper.hpp"
#include <iostream>

void CmdHelper::server_cd(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }
    auto& fs = ServerFsHelper::get_instance();
    fs.cd(arg[1], m_io_context);
}

void CmdHelper::server_mkdir(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }
    auto& fs = ServerFsHelper::get_instance();
    fs.mkdir(arg[1], m_io_context);
}

void CmdHelper::server_rmdir(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }
    auto& fs = ServerFsHelper::get_instance();
    fs.rmdir(arg[1], m_io_context);
}

void CmdHelper::server_rm(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }
    auto& fs = ServerFsHelper::get_instance();
    fs.rm(arg[1], m_io_context);
}

void CmdHelper::server_ls(){
    auto& fs = ServerFsHelper::get_instance();
    fs.ls(m_io_context);
}


void CmdHelper::download(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }

    auto& fs = ServerFsHelper::get_instance();
    fs.download(arg[1], m_io_context);
}

/*
void CmdHelper::upload(const std::vector<std::string>& arg){
    if(arg.size() == 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }

    auto& fs = ServerFsHelper::get_instance();
    fs.upload(arg[1], m_io_context);
}
*/