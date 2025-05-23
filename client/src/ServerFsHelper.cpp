#include "../include/ServerFsHelper.hpp"
#include "../include/Session.hpp"
#include <iostream>
#include <mutex>

std::once_flag ServerFsHelper::m_init_flag;
std::unique_ptr<ServerFsHelper> ServerFsHelper::m_instance;

/*
    std::filesystem::path cwd() const;
    void set_cwd(const std::filesystem::path& cwd);
    void mkdir(const std::filesystem::path& cwd);
    void rmdir(const std::filesystem::path& cwd);
    void ls() const;
*/

ServerFsHelper& ServerFsHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        m_instance = std::make_unique<ServerFsHelper>();
        m_instance->m_working_path = "/";
    });
    return *m_instance;
}

void ServerFsHelper::mkdir(
    const std::filesystem::path& path, const std::string& id,
    boost::asio::io_context& io_context
){
    std::promise <http::response<http::string_body>> prom;
    std::future <http::response<http::string_body>> fut = prom.get_future();
    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/mkdir", {{"id", id}, {"path", path}}
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