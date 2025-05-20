#include "../../include/CmdHelper.hpp"
#include <iostream>

CmdHelper::CmdHelper(boost::asio::io_context& io_context) 
  :  m_io_context(io_context){
}

void CmdHelper::get_cmd(){
    if(logged_in){
        auto cwd = FsHelper::get_instance().cwd().string();
        if(cwd.back() == '/' && cwd.size() > 1){
            cwd.pop_back();
        }
        std::cout << cwd << "$ ";
    }
    else{
        std::cout << "> ";
    }

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

