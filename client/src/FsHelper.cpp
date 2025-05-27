#include "../include/FsHelper.hpp"
#include <iostream>

std::once_flag FsHelper::m_init_flag;
std::unique_ptr<FsHelper> FsHelper::m_instance;

FsHelper& FsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<FsHelper>();
        const char* home = std::getenv("HOME");
        m_instance->m_working_path = home ? home : "/";
    });
    return *m_instance;
}

std::filesystem::path FsHelper::cwd() const {
    return m_working_path.string();
}

void FsHelper::set_cwd(const std::filesystem::path& cwd){
    m_working_path = cwd;
}

bool FsHelper::exists(const std::filesystem::path& cwd) const{
    return std::filesystem::exists(cwd);
}

void FsHelper::mkdir(const std::filesystem::path& cwd){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    if(!std::filesystem::create_directories(path)){
        std::cout << "이미 존재하는 디렉토리입니다." << std::endl;
    }
}

void FsHelper::rmdir(const std::filesystem::path& cwd){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    if(!std::filesystem::exists(path)){
        std::cout << "대상 디렉토리가 존재하지 않습니다." << std::endl;
    }
    else if(!std::filesystem::is_directory(path)){
        std::cout << "대상이 디렉토리가 아닙니다." << std::endl;
    }
    else if(!std::filesystem::is_empty(path)){
        std::cout << "빈 디렉토리가 아닙니다." << std::endl;
    }
    else{
        std::filesystem::remove(path);
    }
}

void FsHelper::rm(const std::filesystem::path& cwd){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    if(!std::filesystem::exists(path)){
        std::cout << "대상 파일이 존재하지 않습니다." << std::endl;
    }
    else if(std::filesystem::is_directory(path)){
        std::cout << "대상이 파일이 아닙니다." << std::endl;
    }
    else{
        std::filesystem::remove(path);
    }
}

void FsHelper::ls() const{
    for(const auto& cur : std::filesystem::directory_iterator(m_working_path)){
        if(cur.is_directory()){
            std::cout << "[DIR] ";
        }
        else{
            std::cout << "[FILE] ";
        }
        std::cout << cur.path().filename().string() << std::endl;
    }
}