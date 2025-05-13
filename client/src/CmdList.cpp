#include "../include/CmdHelper.hpp"
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
    if(!std::filesystem::exists(next_path)){
        std::cout << "존재하지 않는 경로입니다." << std::endl;
    }
    else{
        fs.set_cwd(next_path);
    }
}