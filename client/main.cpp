#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "include/CmdHelper.hpp"
#include "include/FsHelper.hpp"

int main(){
    auto cwd = std::filesystem::current_path().root_directory();
    CmdHelper cmd_helper;
    while(1){
        std::cout << cwd.string() << "$ ";
        cmd_helper.get_cmd();

        if(arg.size() >= 2 && arg[0] == "cd"){
            std::filesystem::path next_path = (cwd / arg[1]).lexically_normal();
            if(!std::filesystem::exists(next_path)){
                std::cout << "존재하지 않는 경로입니다." << std::endl;
            }
            else{
                cwd = next_path;
            }
        }
    }
}