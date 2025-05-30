#include "../include/Service.hpp"
#include "../include/ServerFsHelper.hpp"
#include <iostream>

void Service::sign_in(const json& json, const std::string& id){
    std::string path = json.value("path", "");

    std::cout << "로그인 성공" << std::endl;
    auto& fs = ServerFsHelper::get_instance();
    fs.set_cwd(path);
    fs.set_id(id);
}

void Service::sign_up(const json& json){
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 아이디입니다." << std::endl;
    }
    else{
        std::cout << "회원가입 성공" << std::endl;
    }
}

void Service::server_mkdir(const json& json){
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 디렉토리입니다." << std::endl;
    }
}

void Service::server_cd(const json& json){
    bool ret = json.value("result", 0);
    std::string ret_path = json.value("path", "");
    if(!ret){
        std::cout << "대상 디렉토리가 없습니다." << std::endl;
    }
    else{
        auto& fs = ServerFsHelper::get_instance();
        fs.set_cwd(ret_path);
    }
}

void Service::server_ls(const json& json){
    auto file_name = json["file_name"].get<std::vector<std::string>>();
    auto is_dir = json["is_dir"].get<std::vector<bool>>();
    
    std::vector<std::pair<std::string, bool>> arr;
    for(int i = 0;i < file_name.size();i++){
        arr.push_back({file_name[i], is_dir[i]});
    }

    for(const auto& [file_name, is_dir] : arr){
        if(is_dir){
            std::cout << "[DIR] ";
        }
        else{
            std::cout << "[FILE] ";
        }
        std::cout << file_name << std::endl;
    }
}

void Service::server_rmdir(const json& json){
    int32_t ret = json.value("result", -1);

    if(ret == -1){
        std::cout << "서버 응답 오류" << std::endl;
    }
    else if(ret == 1){
        std::cout << "대상 디렉토리가 존재하지 않습니다." << std::endl;
    }
    else if(ret == 2){
        std::cout << "대상이 디렉토리가 아닙니다" << std::endl;
    }
    else if(ret == 3){
        std::cout << "빈 디렉토리가 아닙니다." << std::endl;
    }
}

void Service::server_rm(const json& json){
    int32_t ret = json.value("result", -1);

    if(ret == -1){
        std::cout << "서버 응답 오류" << std::endl;
    }
    else if(ret == 1){
        std::cout << "대상 파일이 존재하지 않습니다." << std::endl;
    }
    else if(ret == 2){
        std::cout << "대상이 파일이 아닙니다." << std::endl;
    }
}