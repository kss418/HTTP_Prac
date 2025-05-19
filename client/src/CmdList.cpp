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

void CmdHelper::sign_in(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return;
    }
    
    std::promise <http::response <http::string_body>> prom;
    std::future <http::response <http::string_body>> fut = prom.get_future();

    Session session(m_io_context, "127.0.0.1", 8080, prom, http::verb::post, "/login", {{"id", arg[1]}, {"pw", arg[2]}});
    
    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    nlohmann::json json = nlohmann::json::parse(res.body());
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "아이디 또는 비밀번호가 일치하지 않습니다." << std::endl;
    }
    else{
        std::cout << "로그인 성공" << std::endl;
    }
}

void CmdHelper::sign_up(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return;
    }

    std::promise <http::response <http::string_body>> prom;
    std::future <http::response <http::string_body>> fut = prom.get_future();

    Session session(m_io_context, "127.0.0.1", 8080, prom, http::verb::post, "/register", {{"id", arg[1]}, {"pw", arg[2]}});
    
    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    nlohmann::json json = nlohmann::json::parse(res.body());
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 아이디입니다." << std::endl;
    }
    else{
        std::cout << "회원가입 성공" << std::endl;
    }
}

