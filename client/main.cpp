#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

int main(){
    auto cwd = std::filesystem::current_path().root_directory();
    while(1){
        std::cout << cwd << "$ ";
        std::string cmd;
        std::getline(std::cin, cmd);
        
        std::vector <std::string> arg;
        std::string now;
        for(auto& i : cmd){
            if(i == ' '){
                if(!now.empty()) arg.push_back(now);
                now.clear();
            }
            else{
                now.push_back(i);
            }
        }

        if(!now.empty()) arg.push_back(now);
        
    }
}