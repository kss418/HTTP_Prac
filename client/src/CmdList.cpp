#include "../include/CmdHelper.hpp"
#include "../include/Session.hpp"
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

bool CmdHelper::sign_in(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return 0;
    }
    
    std::promise <http::response <http::string_body>> prom;
    std::future <http::response <http::string_body>> fut = prom.get_future();

    Session session(m_io_context, "127.0.0.1", 8080, prom, http::verb::post, "/login", {{"id", arg[1]}, {"pw", arg[2]}});
    
    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return 0;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return 0;
    }

    nlohmann::json json = nlohmann::json::parse(res.body());
    return json["result"].get<bool>();
}

int8_t CmdHelper::sign_up(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return 0;
    }

    std::promise <http::response <http::string_body>> prom;
    std::future <http::response <http::string_body>> fut = prom.get_future();

    Session session(m_io_context, "127.0.0.1", 8080, prom, http::verb::post, "/login", {{"id", arg[1]}, {"pw", arg[2]}});
    
    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return 0;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return 0;
    }

    nlohmann::json json = nlohmann::json::parse(res.body());
    return 0;
}