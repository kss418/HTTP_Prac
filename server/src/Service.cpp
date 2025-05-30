#include "../include/Session.hpp"
#include "../include/DBHelper.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

void Service::sign_in(const json& json, Session_ptr self){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");

    std::string cwd = Service::cwd(json);
    if(db_helper.match_pw(id, pw)){
        self->write_string(
            http::status::ok, 
            {{"path", cwd}}
        );
    }   
    else{
        self->write_string(
            http::status::unauthorized, 
            {{"message", "아이디 또는 비밀번호가 일치하지 않습니다."}}
        );
    }
}

void Service::sign_up(const json& json, Session_ptr self){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");

    if(id.empty() || pw.empty()){
        self->write_string(
            http::status::bad_request, 
            {{"message", "요청 형식이 잘못 되었습니다."}}
        );
    }
    else if(!db_helper.create_id(id, pw)){
        self->write_string(
            http::status::conflict, 
            {{"message", "이미 존재하는 아이디입니다."}}
        );
    }
    else{
        self->write_empty(http::status::ok);
    }
}

void Service::mkdir(const json& json, Session_ptr self){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    
    auto& executer = fs.get_executer(id);
    if(executer.mkdir(path)){
        self->write_empty(http::status::ok);
    }
    else{
        self->write_string(
            http::status::conflict, 
            {{"message", "해당 이름을 가진 폴더나 파일이 존재합니다."}}
        );
    }
}

bool Service::cd(const json& json){
    auto& fs = FsHelper::get_instance();
    std::string id = json.value("id", "");
    std::filesystem::path path = json.value("path", "");
    
    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }
    std::filesystem::path next_path = ((fs.m_map[id])->cwd() / path).lexically_normal();

    std::cout << "cd / id = " << id << " path = " << next_path << std::endl;
    if(!std::filesystem::exists(next_path)){
        return 0;
    }
    (fs.m_map[id])->set_cwd(next_path);
    return 1;
}

std::string Service::cwd(const json& json){
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

int32_t Service::rmdir(const json& json){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    if(path.empty()) return -1;

    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->rmdir(path);
}

int32_t Service::rm(const json& json){
    auto& fs = FsHelper::get_instance();
    std::string path = json.value("path", "");
    std::string id = json.value("id", "");
    if(path.empty()) return -1;

    if(fs.m_map.find(id) == fs.m_map.end()){
        fs.m_map[id] = std::make_unique<FsExecuter>(id);
    }

    return (fs.m_map[id])->rm(path);
}

void Service::download(
    const std::string& id, const std::filesystem::path& path, 
    Session_ptr self
){
    auto& fs = FsHelper::get_instance();
    auto cwd = (fs.m_map[id])->cwd();
    std::string full_path = path.is_absolute() ? path : cwd / path;

    if(!std::filesystem::exists(full_path)){
        self->write_string(
            http::status::not_found, 
            {{"message", "해당 파일이 존재하지 않습니다."}}
        );
    }
    else{
        self->write_file(http::status::ok, full_path);
    }   
}

void Service::upload(
    const std::string& id, const std::filesystem::path& path, 
    Session_ptr self
){
    auto& fs = FsHelper::get_instance();
    auto cwd = (fs.m_map[id])->cwd();
    std::string full_path = path.is_absolute() ? path : cwd / path;

    if(std::filesystem::exists(full_path)){
        self->write_string(
            http::status::conflict, 
            {{"message", "중복된 이름의 파일이 존재합니다."}}
        );
    }
    else{
        self->read_file(path);
        self->write_empty(http::status::ok);
    }   
}