#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"
#include "../../include/Service.hpp"
#include "../../include/Utility.hpp"
#include <iostream>
#include <expected>

void CmdHelper::sign_in(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return;
    }

    auto session = std::make_shared<Session>(
        m_io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [session, &arg]{
            session->write_string(
                http::verb::post, "/login",
                {{"id", arg[1]}, {"pw", arg[2]}}
            );
        }
    );
    write_fut.get();

    std::future<var_parser> read_fut = std::async(
        std::launch::async, [session]{
            return session->read();
        }
    );
    auto var = read_fut.get();

    if(std::holds_alternative<string_parser>(var)){
        auto res = std::get<string_parser>(var)->get();
        auto json = Utility::parse_json(res.body());
        if(!json){
            std::cerr << json.error() << std::endl;
            return;
        }

        if(res.result() != http::status::ok){
            auto message = json->value("message", "");
            std::cout << message << std::endl;
            return;
        }

        Service::sign_in(*json, arg[1], logged_in);
    }
    else{
        std::cout << "서버 응답 오류" << std::endl;
    }
}

void CmdHelper::sign_up(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return;
    }

    if(arg[1].size() >= 20){
        std::cout << "아이디의 길이는 최대 20까지 설정 할 수 있습니다." << std::endl;
        return;
    }

    auto session = std::make_shared<Session>(
        m_io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [session, &arg]{
            session->write_string(
                http::verb::post, "/register",
                {{"id", arg[1]}, {"pw", arg[2]}}
            );
        }
    );
    write_fut.get();

    std::future<var_parser> read_fut = std::async(
        std::launch::async, [session]{
            return session->read();
        }
    );
    auto var = read_fut.get();

   if(std::holds_alternative<string_parser>(var)){
        auto res = std::get<string_parser>(var)->get();
        auto json = Utility::parse_json(res.body());
        if(!json){
            std::cerr << json.error() << std::endl;
            return;
        }

        if(res.result() != http::status::ok){
            auto message = json->value("message", "");
            std::cout << message << std::endl;
            return;
        }
    }
    else if(std::holds_alternative<empty_parser>(var)){
        auto res = std::get<empty_parser>(var)->get();
        Service::sign_up();
    }
    else{
        std::cout << "서버 응답 오류" << std::endl;
    }
}

void CmdHelper::logout(){
    std::cout << "로그아웃" << std::endl;
    id.clear();

    logged_in = 0;
}