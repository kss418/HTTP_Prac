#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"
#include <iostream>

void CmdHelper::cd(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    if(arg.size() <= 1){
        const char* home = std::getenv("HOME");
        fs.set_cwd(home ? home : "/");
        return;
    }

    if(arg[1] == "." || arg[1] == "./"){
        return;
    }

    std::filesystem::path next_path = (fs.cwd() / arg[1]).lexically_normal();
    if(!fs.exists(next_path)){
        std::cout << "존재하지 않는 경로입니다." << std::endl;
    }
    else{
        fs.set_cwd(next_path);
    }
}

void CmdHelper::mkdir(const std::vector<std::string>& arg){
    if(arg.size() <= 1){
        std::cout << "인자가 없습니다." << std::endl;
        return;
    }

    auto& fs = FsHelper::get_instance();
    fs.mkdir(arg[1]); 
}

void CmdHelper::ls(){
    auto& fs = FsHelper::get_instance();
    fs.ls();
}

void CmdHelper::rmdir(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    fs.rmdir(arg[1]);
}

void CmdHelper::set_upload_file(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    if(arg.size() <= 1){
        const char* home = std::getenv("HOME");
        fs.set_cwd(home ? home : "/");
        return;
    }

    std::filesystem::path next_path = (fs.cwd() / arg[1]);
    if(!fs.exists(next_path)){
        std::cout << "대상이 없습니다." << std::endl;
        return;
    }

    if(std::filesystem::is_directory(next_path)){
        std::cout << "대상이 디렉토리입니다." << std::endl;
        return;
    }
    
    local_path = next_path;

}

void CmdHelper::set_download_dir(const std::vector<std::string>& arg){
    auto& fs = FsHelper::get_instance();
    if(arg.size() <= 1){
        const char* home = std::getenv("HOME");
        fs.set_cwd(home ? home : "/");
        return;
    }

    std::filesystem::path next_path = (fs.cwd() / arg[1]);
    if(!fs.exists(next_path)){
        std::cout << "대상이 없습니다." << std::endl;
        return;
    }

    if(!std::filesystem::is_directory(next_path)){
        std::cout << "대상이 파일입니다." << std::endl;
        return;
    }

    std::promise <http::response <http::string_body>> prom;
    std::future <http::response <http::string_body>> fut = prom.get_future();
    Session session(
        m_io_context, "127.0.0.1", 8080,
        prom, http::verb::get, "/donwload?path=" + server_path
    );

    fut.wait();

    local_path.clear();
    server_path.clear();

}

void CmdHelper::change(){
    path_state ^= 1;
}