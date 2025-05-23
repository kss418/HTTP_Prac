#include "../include/Service.hpp"
#include "../include/DBHelper.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

bool Service::sign_in(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");
    
    return db_helper.match_pw(id, pw);
}

bool Service::sign_up(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");

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

    return (fs.m_map[id])->mkdir(path);
}

std::string Service::download(json json){
    return "";
}