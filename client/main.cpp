#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include "include/CmdParser.hpp"

int main(){
    auto cwd = std::filesystem::current_path().root_directory();
    CmdParser cp;
    while(1){
        std::cout << cwd << "$ ";
        std::vector <std::string> arg = cp.get_parsed_cmd();

        for(auto& i : arg) std::cout << i << " ";
        std::cout << std::endl;
    }
}