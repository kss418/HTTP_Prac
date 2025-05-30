#include "../include/FsHelper.hpp"
#include <iostream>

std::once_flag FsHelper::m_init_flag;
std::unique_ptr<FsHelper> FsHelper::m_instance;

FsHelper& FsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<FsHelper>();
    });
    return *m_instance;
}

FsExecuter::FsExecuter(const std::string& id)
  :  m_working_path(std::filesystem::current_path().root_directory() / "home" / "kss418" / id){  
}

std::filesystem::path FsExecuter::cwd() const{
    return m_working_path.string();
}

void FsExecuter::set_cwd(const std::filesystem::path& dir_path){
    m_working_path = dir_path;
}

bool FsExecuter::mkdir(const std::filesystem::path& any_path){
    const auto next_path = any_path.is_absolute() 
        ? any_path : (m_working_path / any_path);
    return std::filesystem::create_directories(next_path);
}

int32_t FsExecuter::rmdir(const std::filesystem::path& any_path){
    const auto next_path = any_path.is_absolute() 
        ? any_path : (m_working_path / any_path);
    if(!std::filesystem::exists(next_path)){
        return 1;
    }
    else if(!std::filesystem::is_directory(next_path)){
        return 2;
    }
    else if(!std::filesystem::is_empty(next_path)){
        return 3;
    }
    else{
        std::filesystem::remove(next_path);
        return 0;
    }
}

int32_t FsExecuter::rm(const std::filesystem::path& any_path){
    const auto next_path = any_path.is_absolute() 
        ? any_path : (m_working_path / any_path);
    if(!std::filesystem::exists(next_path)){
        return 1;
    }
    else if(std::filesystem::is_directory(next_path)){
        return 2;
    }
    else{
        std::filesystem::remove(next_path);
        return 0;
    }
}

nlohmann::json FsExecuter::ls() const{
    nlohmann::json json;
    json["file_name"] = nlohmann::json::array();
    json["is_dir"] = nlohmann::json::array();
    for(const auto& cur : std::filesystem::directory_iterator(cwd())){
        json["file_name"].push_back(cur.path().filename().string());
        json["is_dir"].push_back(cur.is_directory());
    }

    return json;
}
