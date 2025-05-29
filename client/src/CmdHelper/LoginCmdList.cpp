#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"
#include "../../include/FsHelper.hpp"
#include "../../include/ServerFsHelper.hpp"
#include "../../include/Service.hpp"
#include <iostream>

void CmdHelper::sign_in(const std::vector<std::string>& arg){
    if(arg.size() <= 2){
        std::cout << "아이디와 비밀번호를 입력해주세요." << std::endl;
        return;
    }

    auto session = std::make_shared<Session>(
        m_io_context, boost::asio::ip::make_address("127.0.0.1"), 8080
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
        return;
    }
    auto res = std::get<string_parser>(var)

    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    nlohmann::json json = nlohmann::json::parse(res->body());
    
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

    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(m_io_context, "127.0.0.1", 8080, prom, http::verb::post, "/register", {{"id", arg[1]}, {"pw", arg[2]}});
    
    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    nlohmann::json json = nlohmann::json::parse(res->body());
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 아이디입니다." << std::endl;
    }
    else{
        std::cout << "회원가입 성공" << std::endl;
    }
}

void CmdHelper::logout(){
    std::cout << "로그아웃" << std::endl;
    id.clear();
    logged_in = 0;
}