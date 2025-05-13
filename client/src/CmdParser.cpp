#include "../include/CmdParser.hpp"
#include <iostream>

CmdParser::CmdParser() {}

std::vector<std::string> CmdParser::get_parsed_cmd(){
    std::string input;
    std::getline(std::cin, input);
    return parse_cmd(input);
}

std::vector<std::string> CmdParser::parse_cmd(const std::string& input){
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