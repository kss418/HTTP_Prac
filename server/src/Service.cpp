#include "../include/Session.hpp"
#include "../include/DBHelper.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

bool Service::sign_in(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");
    
    std::cout << "로그인 / id = " << id << std::endl;
    return db_helper.match_pw(id, pw);
}

bool Service::sign_up(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");

    std::cout << "회원가입 / id = " << id << std::endl;
    if(id.empty() || pw.empty()){
        return 0;
    }
    
    return db_helper.create_id(id, pw);
}

bool Service::mkdir(json json){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    
    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    std::cout << "mkdir / id = " << id << " path = " << path << std::endl;
    return (fs.m_map[id])->mkdir(path);
}

bool Service::cd(json json){
    auto& fs = FsHelper::get_instance();
    std::string id = json.value("id", "");
    std::filesystem::path path = json.value("path", "");
    
    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }
    std::filesystem::path next_path = ((fs.m_map[id])->cwd() / path).lexically_normal();

    std::cout << "cd / id = " << id << " path = " << next_path << std::endl;
    if(!(fs.m_map[id])->exists(next_path)){
        return 0;
    }
    return (fs.m_map[id])->set_cwd(next_path);
}

std::string Service::cwd(json json){
    auto& fs = FsHelper::get_instance();
    std::string id = json.value("id", "");
    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->cwd();
}

nlohmann::json Service::ls(const std::string& id){
    auto& fs = FsHelper::get_instance();
    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->ls();
}

int32_t Service::rmdir(json json){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    if(path.empty()) return -1;

    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->rmdir(path);
}

int32_t Service::rm(json json){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    if(path.empty()) return -1;

    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->rm(path);
}

void upload(
    const std::string& id, const std::string& path, 
    const std::string& file_name
){
    
}