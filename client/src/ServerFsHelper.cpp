#include "../include/ServerFsHelper.hpp"
#include "../include/Session.hpp"
#include <iostream>
#include <mutex>

std::once_flag ServerFsHelper::m_init_flag;
std::unique_ptr<ServerFsHelper> ServerFsHelper::m_instance;

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
    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/mkdir", {{"id", m_id}, {"path", path}}
    );

    
    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    auto json = nlohmann::json::parse(res->body());
    bool ret = json.value("result", 0);
    if(!ret){
        std::cout << "이미 존재하는 디렉토리입니다." << std::endl;
    }
}

void ServerFsHelper::cd(
    const std::filesystem::path& path, 
    boost::asio::io_context& io_context
){
    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/cd", {{"id", m_id}, {"path", path}}
    );

    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    auto json = nlohmann::json::parse(res->body());
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
    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::get, "/ls?id=" + m_id
    );
    
    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return {};
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return {};
    }

    auto json = nlohmann::json::parse(res->body());
    auto file_name = json["file_name"].get<std::vector<std::string>>();
    auto is_dir = json["is_dir"].get<std::vector<bool>>();
    
    std::vector<std::pair<std::string, bool>> ret;
    for(int i = 0;i < file_name.size();i++){
        ret.push_back({file_name[i], is_dir[i]});
    }

    return ret;
}

void ServerFsHelper::rmdir(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::delete_, "/rmdir", {{"id", m_id}, {"path", cwd}}
    );

    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    auto json = nlohmann::json::parse(res->body());
    int32_t ret = json.value("result", -1);

    if(ret == -1){
        std::cout << "서버 응답 오류" << std::endl;
    }
    else if(ret == 1){
        std::cout << "대상 디렉토리가 존재하지 않습니다." << std::endl;
    }
    else if(ret == 2){
        std::cout << "대상이 디렉토리가 아닙니다" << std::endl;
    }
    else if(ret == 3){
        std::cout << "빈 디렉토리가 아닙니다." << std::endl;
    }
}

void ServerFsHelper::rm(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    std::promise <std::shared_ptr<http::response <http::string_body>>> prom;
    std::future <std::shared_ptr<http::response <http::string_body>>> fut = prom.get_future();

    Session session(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::delete_, "/rm", {{"id", m_id}, {"path", cwd}}
    );
    
    std::shared_ptr<http::response <http::string_body>> res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

    if(!nlohmann::json::accept(res->body())){
        std::cout << "서버 응답 파싱 불가" << std::endl;
        return;
    }

    auto json = nlohmann::json::parse(res->body());
    int32_t ret = json.value("result", -1);

    if(ret == -1){
        std::cout << "서버 응답 오류" << std::endl;
    }
    else if(ret == 1){
        std::cout << "대상 파일이 존재하지 않습니다." << std::endl;
    }
    else if(ret == 2){
        std::cout << "대상이 파일이 아닙니다." << std::endl;
    }
}

void ServerFsHelper::download(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    using res_ptr = std::shared_ptr<
        http::response<http::file_body>
    >;

    std::promise <res_ptr> prom;
    std::future <res_ptr> fut = prom.get_future();
    auto session = std::make_shared<Session<http::file_body>>(
        io_context, "127.0.0.1", 8080, 
        prom, http::verb::get, "/download?id=" + m_id + "&path=" + path.string()
    );

    res_ptr res = fut.get();
    if(res->result() != http::status::ok){
        std::cout << "Status code : " << res->result() << std::endl;
        return;
    }

}


