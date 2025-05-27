#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"
#include <iostream>

void CmdHelper::cd(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    if(arg.size() <= 1){
        const char* home = std::getenv("HOME");
        fs.set_cwd(home ? home : "/");
        return;
    }

    if(arg[1] == "." || arg[1] == "./"){
        return;
    }

    std::filesystem::path next_path = (fs.cwd() / arg[1]).lexically_normal();
    if(!fs.exists(next_path)){
        std::cout << "존재하지 않는 경로입니다." << std::endl;
    }
    else{
        fs.set_cwd(next_path);
    }
}

void CmdHelper::mkdir(const std::vector<std::string>& arg){
    if(arg.size() <= 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }

    auto& fs = FsHelper::get_instance();
    fs.mkdir(arg[1]); 
}

void CmdHelper::ls(){
    auto& fs = FsHelper::get_instance();
    fs.ls();
}

void CmdHelper::rmdir(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    fs.rmdir(arg[1]);
}

void CmdHelper::rm(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    fs.rm(arg[1]);
}

void CmdHelper::change(){
    path_state ^= 1;
}