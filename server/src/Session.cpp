#include "../include/Session.hpp"
#include "../include/Service.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

Session::Session(std::shared_ptr<tcp::socket> socket)
  :  m_socket(std::move(socket)){
}

void Session::read(){
    http::async_read(
        *m_socket,
        m_buffer,
        m_req,
        [self = shared_from_this()](
            const boost::system::error_code& ec,
            std::size_t bytes_transferred
        ){
            self->handle_read(ec);
        }
    );
}

void Session::write(http::status status, const nlohmann::json& json){
    auto res = std::make_shared<http::response<http::string_body>>(
        status, m_req.version()
    );
    res->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    std::string body = json.dump();
    if(!body.empty()){
        res->body() = body;
        res->prepare_payload();
    }

    http::async_write(
        *m_socket,
        *res,
        [self = shared_from_this(), res](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            self->handle_write(ec);
        }
    );
}

void Session::execute_request(){
    auto method = m_req.method();
    auto target = m_req.target();

    auto pos_q = target.find('?');
    std::string path = target.substr(0, pos_q == -1 ? target.size() : pos_q);
    std::string arg = target.substr(pos_q == -1 ? target.size() : pos_q);

    std::cout << path << std::endl;
    std::cout << arg << std::endl;
    std::cout << m_req.body() << std::endl;
    if(method != http::verb::get && !nlohmann::json::accept(m_req.body())){
        std::cout << "파싱 불가" << std::endl;
        std::cout << m_req.body() << std::endl;
        write(http::status::bad_request);
        return;
    }

    nlohmann::json json = (method != http::verb::get ? nlohmann::json::parse(m_req.body()) : json.object());
    if(method == http::verb::post && path == "/login"){
        bool ret = Service::sign_in(json);
        std::string cwd = Service::cwd(json);
        write(http::status::ok, {{"result", ret}, {"path", cwd}});
    }
    else if(method == http::verb::post && path == "/register"){
        bool ret = Service::sign_up(json);
        write(http::status::ok, {{"result", ret}});
    }
    else if(method == http::verb::post && path == "/mkdir"){
        bool ret = Service::mkdir(json);
        write(http::status::ok, {{"result", ret}});
    }
    else if(method == http::verb::delete_ && path == "/rmdir"){
        int32_t ret = Service::rmdir(json);
        write(http::status::ok, {{"result", ret}});
    }
    else if(method == http::verb::delete_ && path == "/rm"){
        int32_t ret = Service::rm(json);
        write(http::status::ok, {{"result", ret}});
    }
    else if(method == http::verb::post && path == "/cd"){
        bool ret = Service::cd(json);
        std::string cwd = Service::cwd(json);
        write(http::status::ok, {{"result", ret}, {"path", cwd}});
    }
    else if(method == http::verb::get && path == "/ls"){
        std::string id = arg.substr(4);
        std::cout << id << std::endl;
        write(http::status::ok, Service::ls(id));
    }
}
