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
        cmd_helper.get_cmd();
        
    }
}