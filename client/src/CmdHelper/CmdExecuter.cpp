#include "../../include/CmdHelper.hpp"
#include <iostream>

void CmdHelper::execute_cmd(const std::vector<std::string>& arg){
    if(arg.empty()){
        return;
    }

    if(logged_in){
        if(!file_state){
            login_execute(arg);
        }
        else if(file_state == 1){
            local_execute(arg);
        }
        else{
            server_execute(arg);
        }
    }
    else{
        logout_execute(arg);
    }
}

void CmdHelper::login_execute(const std::vector<std::string>& arg){
    if(arg[0] == "logout"){
        logout();
    }
    else if(arg[0] == "upload"){
        upload();
    }
    else if(arg[0] == "download"){
        download();
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
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
    else if(arg[0] == "set"){
        set(arg);
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
    }
}

void CmdHelper::server_execute(const std::vector<std::string>& arg){
    if(arg[0] == "cd"){

    }
    else if(arg[0] == "ls"){

    }
    else if(arg[0] == "mkdir"){

    }
    else if(arg[0] == "rmdir"){

    }
    else if(arg[0] == "set"){
        set(arg);
    }
    else{
        std::cout << "존재하지 않는 명령어입니다." << std::endl;
    }
}

void CmdHelper::upload(){
    file_state = 1;
}

void CmdHelper::download(){
    file_state = 2;
}

void CmdHelper::set(const std::vector<std::string>& arg){
    if(load_state){
        std::cout << "업로드 / 다운로드 상태 오류" << std::endl;
    }

    if(file_state == 1){
        if(load_state == 1){
            set_upload_file(arg);
        }
        else{
            set_download_file(arg);
        }
    }
    else if(file_state == 2){
        if(load_state == 1){
            set_upload_dir(arg);
        }
        else {
            set_download_dir(arg);
        }   
    }
    else{
        std::cout << "파일 상태 오류" << std::endl;
    }
}