#include "../include/ServerFsHelper.hpp"
#include "../include/Session.hpp"
#include <iostream>
#include <mutex>

std::once_flag ServerFsHelper::m_init_flag;
std::unique_ptr<ServerFsHelper> ServerFsHelper::m_instance;

/*  
    void rmdir(const std::filesystem::path& cwd);
    void ls() const;
    void cd()
*/

ServerFsHelper& ServerFsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<ServerFsHelper>();
        m_instance->m_working_path = "/";
    });
    return *m_instance;
}

void ServerFsHelper::set_id(const std::string& id){
    m_id = id;
}

void ServerFsHelper::set_cwd(const std::filesystem::path& cwd){
    m_working_path = cwd;
}

std::filesystem::path ServerFsHelper::cwd() const {
    return m_working_path.string();
}

void ServerFsHelper::mkdir(
    const std::filesystem::path& path,
    boost::asio::io_context& io_context
){
    std::promise <http::response<http::string_body>> prom;
    std::future <http::response<http::string_body>> fut = prom.get_future();
    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/mkdir", {{"id", m_id}, {"path", path}}
    );

    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    } 

    auto json = nlohmann::json::parse(res.body());
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 디렉토리입니다." << std::endl;
    }
}

void ServerFsHelper::cd(
    const std::filesystem::path& path, 
    boost::asio::io_context& io_context
){
    std::promise <http::response<http::string_body>> prom;
    std::future <http::response<http::string_body>> fut = prom.get_future();
    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/cd", {{"id", m_id}, {"path", path}}
    );

    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    } 

    auto json = nlohmann::json::parse(res.body());
    bool ret = json.value("result", 0);
    std::string ret_path = json.value("path", "");
    if(!ret){
        std::cout << "대상 디렉토리가 없습니다." << std::endl;
    }
    else{
        set_cwd(ret_path);
    }
}

std::vector<std::pair<std::string, bool>> ServerFsHelper::ls(
    boost::asio::io_context& io_context
){
    std::promise <http::response<http::string_body>> prom;
    std::future <http::response<http::string_body>> fut = prom.get_future();
    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::get, "/ls?id=" + m_id
    );

    http::response <http::string_body> res = fut.get();
    if(res.result() != http::status::ok){
        std::cout << "Status code : " << res.result() << std::endl;
        return {};
    }

    if(!nlohmann::json::accept(res.body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return {};
    }

    auto json = nlohmann::json::parse(res.body());
    std::cout << json << std::endl;
    auto ret = json["result"].get<std::vector<std::pair<std::string, bool>>>();
    return ret;
}