#include "../../include/CmdHelper.hpp"
#include <iostream>

void CmdHelper::execute_cmd(const std::vector<std::string>& arg){
    if(arg.empty()){
        return;
    }

    if(logged_in){
        login_execute(arg);
    }
    else{
        logout_execute(arg);
    }
}

void CmdHelper::login_execute(const std::vector<std::string>& arg){
    if(arg[0] == "logout"){
        logout();
    }
    else if(arg[0] == "download"){
        download(arg);
    }
    else if(arg[0] == "upload"){
        upload(arg);
    }
    else if(!path_state){
        local_execute(arg);
    }
    else{
        server_execute(arg);
    }
}

void CmdHelper::logout_execute(const std::vector<std::string>& arg){
    if(arg[0] == "login"){
        sign_in(arg);
    }
    else if(arg[0] == "register"){
        sign_up(arg);
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
    }
}

void CmdHelper::local_execute(const std::vector<std::string>& arg){
    if(arg[0] == "cd"){
        cd(arg);
    }
    else if(arg[0] == "ls"){
        ls();
    }
    else if(arg[0] == "mkdir"){
        mkdir(arg);
    }
    else if(arg[0] == "rmdir"){
        rmdir(arg);
    }
    else if(arg[0] == "change"){
        change();
    }
    else if(arg[0] == "rm"){
        rm(arg);
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
    }
}

void CmdHelper::server_execute(const std::vector<std::string>& arg){
    if(arg[0] == "cd"){
        server_cd(arg);
    }
    else if(arg[0] == "ls"){
        server_ls();
    }
    else if(arg[0] == "mkdir"){
        server_mkdir(arg);
    }
    else if(arg[0] == "rmdir"){
        server_rmdir(arg);
    }
    else if(arg[0] == "rm"){
        server_rm(arg);
    }
    else if(arg[0] == "change"){
        change();
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
    }
}
