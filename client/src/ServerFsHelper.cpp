#include "../include/ServerFsHelper.hpp"
#include "../include/Session.hpp"
#include "../include/Utility.hpp"
#include "../include/Service.hpp"
#include "../include/FsHelper.hpp"
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
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session, path]{
            session->write_string(
                http::verb::post, "/mkdir",
                {{"id", m_id}, {"path", path}}
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

    if(!std::holds_alternative<string_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
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

    Service::server_mkdir(*json);
}

void ServerFsHelper::cd(
    const std::filesystem::path& path, 
    boost::asio::io_context& io_context
){
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session, path]{
            session->write_string(
                http::verb::post, "/cd",
                {{"id", m_id}, {"path", path}}
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

    if(!std::holds_alternative<string_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
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

    Service::server_cd(*json);
}

void ServerFsHelper::ls(
    boost::asio::io_context& io_context
){  
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session]{
            session->write_empty(
                http::verb::get, "/ls?id=" + m_id
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

    if(!std::holds_alternative<string_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
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

    Service::server_ls(*json);
}

void ServerFsHelper::rmdir(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session, cwd]{
            session->write_string(
                http::verb::delete_, "/rmdir",
                {{"id", m_id}, {"path", cwd}}
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

    if(!std::holds_alternative<string_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
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

    Service::server_rmdir(*json);
}

void ServerFsHelper::rm(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session, cwd]{
            session->write_string(
                http::verb::delete_, "/rm",
                {{"id", m_id}, {"path", cwd}}
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

    if(!std::holds_alternative<string_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
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

    Service::server_rmdir(*json);
}

void ServerFsHelper::download(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    auto session = std::make_shared<Session>(
        io_context, "127.0.0.1", 8080
    );
    
    std::future<void> write_fut = std::async(
        std::launch::async, [this, session, cwd]{
            session->write_string(
                http::verb::get, "/download",
                {{"id", m_id}, {"path", cwd}}
            );
        }
    );
    write_fut.get();

    auto& fs = FsHelper::get_instance();
    std::future<var_parser> read_fut = std::async(
        std::launch::async, [session, &fs]{
            return session->read(fs.cwd());
        }
    );
    auto var = read_fut.get();

    if(!std::holds_alternative<file_parser>(var)){
        std::cout << "서버 응답 오류" << std::endl;
        return;
    }
}

/*
void ServerFsHelper::upload(
    const std::filesystem::path& cwd,
    boost::asio::io_context& io_context
){
    const auto path = cwd.is_absolute() ? cwd : (m_working_path / cwd);
    if(exist(path, io_context)){
        return;
    }

    const std::string file_name = path.filename();
    const std::string target = "/upload?id=" + m_id + "&path=" + path.string();
    auto file_session = std::make_shared<Upload>(
        io_context, "127.0.0.1", 8080, 
        http::verb::post, target, file_name
    );
    file_session->connect();
}
*/



