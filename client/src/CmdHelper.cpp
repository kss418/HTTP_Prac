#include "../include/CmdHelper.hpp"
#include <iostream>

CmdHelper::CmdHelper() {}

void CmdHelper::get_cmd(){
    std::string input;
    std::getline(std::cin, input);
    execute_cmd(parse_cmd(input));
}

std::vector<std::string> CmdHelper::parse_cmd(const std::string& input){
    std::vector<std::string> arg;
    std::string now;

    for(auto& i : input){
        if(i == ' '){
            if(!now.empty()){
                arg.push_back(std::move(now));
            }
            now.clear();
        }
        else{
            now.push_back(i);
        }
    }

    if(!now.empty()){
        arg.push_back(std::move(now));
    }

    return arg;
}

void CmdHelper::execute_cmd(const std::vector<std::string>& arg){

}