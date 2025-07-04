#include "../include/Session.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>
#include <boost/url.hpp>
#include <unordered_map>

Session::Session(std::shared_ptr<tcp::socket> socket)
  :  m_socket(std::move(socket)),
    m_req_header(std::make_shared<http::request_parser<http::empty_body>>())
{
}

void Session::read_header(){
    http::async_read_header(
        *m_socket,
        m_buffer,
        *m_req_header,
        [self = shared_from_this()](
            const boost::system::error_code& ec,
            std::size_t bytes_transferred
        ){
            self->handle_read_header(ec);
        }
    );
}

void Session::read_string(){
    auto const& header = m_req_header->get();
    auto len_sv = header[boost::beast::http::field::content_length];
    std::size_t len = len_sv.empty() ? 0 : std::stoull(std::string(len_sv));

    auto parser = std::make_shared<http::request_parser<http::string_body>>(std::move(*m_req_header));
    parser->get().body().reserve(len);

    http::async_read(
        *m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec, std::size_t bytes
        ){
            self->handle_read_string(ec, parser);
        }
    );
}

void Session::read_empty(){
    auto parser = m_req_header;

    http::async_read(
        *m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec, std::size_t bytes
        ){
            self->handle_read_empty(ec, parser);
        }
    );
}

void Session::read_file(const std::string& full_path){
    auto const& header = m_req_header->get();
    auto parser = std::make_shared<http::request_parser<http::file_body>>(std::move(*m_req_header));

    boost::system::error_code ec;
    parser->get().body().open(
        full_path.c_str(),
        boost::beast::file_mode::write, ec
    );

    if(ec){
        std::cout << "업로드 실패 : " << ec.message() << std::endl;
        return;
    }

    http::async_read(
        *m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec, std::size_t bytes
        ){
            self->handle_read_file(ec);
        }
    );
}

void Session::write_empty(http::status status){
    auto res = std::make_shared<http::response<http::empty_body>>(
        status, 11
    );
    res->set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res->set("X-Body-Type", "empty_body");

    http::async_write(
        *m_socket,
        *res,
        [self = shared_from_this(), res](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            self->handle_write(ec, "empty_body");
        }
    );
}

void Session::write_string(http::status status, const nlohmann::json& json){
    auto res = std::make_shared<http::response<http::string_body>>(
        status, 11
    );
    res->set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res->set("X-Body-Type", "string_body");

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
            self->handle_write(ec, "string_body");
        }
    );
}

void Session::write_file(
    http::status status, const std::filesystem::path& full_path
){
    boost::beast::error_code ec;
    auto res = std::make_shared<http::response<http::file_body>>(
        http::status::ok, 11
    );

    res->set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res->body().open(full_path.c_str(), boost::beast::file_mode::scan, ec);
    res->set("X-File-Name", full_path.filename().string());
    res->set("X-Body-Type", "file_body");

    if(ec == boost::beast::errc::no_such_file_or_directory){
        write_string(
            http::status::not_found,
            {{"message", "해당 파일이 존재하지 않습니다."}}
        );
        return;
    }

    if(ec){
        write_string(
            http::status::internal_server_error,
            {{"message", "내부 서버 오류가 발생했습니다."}}
        );
        return;
    }
    res->prepare_payload();

    http::async_write(
        *m_socket,
        *res,
        [self = shared_from_this(), res](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            self->handle_write(ec, "file_body");
        }
    );
}

void Session::execute_string_request(
    std::shared_ptr<http::request_parser<http::string_body>> parser
){
    const auto& req = parser->get();
    auto method = req.method();
    auto target = req.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write_empty(http::status::bad_request);
        return;
    }

    std::cout << target << std::endl;
    std::cout << req.body() << std::endl;
    if(!nlohmann::json::accept(req.body())){
        std::cout << "body 파싱 불가" << std::endl;
        std::cout << req.body() << std::endl;
        write_empty(http::status::bad_request);
        return;
    }

    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
    }

    auto self = shared_from_this();
    nlohmann::json json = nlohmann::json::parse(req.body());
    if(method == http::verb::post && path == "/login"){
        Service::sign_in(json, self);
    }
    else if(method == http::verb::post && path == "/register"){
        Service::sign_up(json, self);
    }
    else if(method == http::verb::post && path == "/mkdir"){
        Service::mkdir(json, self);
    }
    else if(method == http::verb::delete_ && path == "/rmdir"){
        Service::rmdir(json, self);
    }
    else if(method == http::verb::delete_ && path == "/rm"){
        Service::rm(json, self);
    }
    else if(method == http::verb::post && path == "/cd"){
        bool ret = Service::cd(json);
        std::string cwd = Service::cwd(json);
        write_string(http::status::ok, {{"result", ret}, {"path", cwd}});
    }
    else{
        write_empty(http::status::bad_request);
    }
}

void Session::execute_empty_request(
    std::shared_ptr<http::request_parser<http::empty_body>> parser
){
    const auto& req = parser->get();
    auto method = req.method();
    auto target = req.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write_string(
            http::status::bad_request,
            {{"message", "올바르지 않은 URI 입니다."}}
        );
        return;
    }

    std::cout << target << std::endl;
    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
    }

    if(method == http::verb::get && path == "/ls"){
        if(map.find("id") == map.end()){
            write_empty(http::status::bad_request);
            return;
        }
        write_string(http::status::ok, Service::ls(map["id"]));
    }
    else if(method == http::verb::get && path == "/download"){
        if(map.find("id") == map.end() || map.find("path") == map.end()){
            write_empty(http::status::bad_request);
            return;
        }
        Service::download(map["id"], map["path"], shared_from_this());
    }
    else{
        write_empty(http::status::bad_request);
    }
}

void Session::execute_file_request(){
    const auto& req_header = m_req_header->get();
    auto method = req_header.method();
    auto target = req_header.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write_empty(http::status::bad_request);
        return;
    }

    std::cout << target << std::endl;
    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
    }

    if(method == http::verb::post && path == "/upload"){
        if(map.find("id") == map.end() || map.find("path") == map.end()){
            write_empty(http::status::bad_request);
            return;
        }

    }
    else{
        write_empty(http::status::bad_request);
    }
}
