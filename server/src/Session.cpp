#include "../include/Session.hpp"
#include "../include/Service.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>
#include <boost/url.hpp>
#include <unordered_map>

Session::Session(std::shared_ptr<tcp::socket> socket)
  :  m_socket(std::move(socket)){
}

void Session::read_header(){
    http::async_read_header(
        *m_socket,
        m_buffer,
        m_req_header,
        [self = shared_from_this()](
            const boost::system::error_code& ec,
            std::size_t bytes_transferred
        ){
            self->handle_read_header(ec);
        }
    );
}

void Session::read_string(){
    auto const& header = m_req_header.get();
    auto len_sv = header[boost::beast::http::field::content_length];
    std::size_t len = len_sv.empty() ? 0 : std::stoull(std::string(len_sv));

    auto parser = std::make_shared<http::request_parser<http::string_body>>();
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
    auto const& header = m_req_header.get();
    auto parser = std::make_shared<http::request_parser<http::empty_body>>();

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

void Session::write(http::status status, const nlohmann::json& json){
    auto res = std::make_shared<http::response<http::string_body>>(
        status, m_req.version()
    );
    res->set(http::field::server, BOOST_BEAST_VERSION_STRING);

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

void Session::write_file(
    http::status status,
    const std::string& id,
    const std::filesystem::path& cwd
){
    auto& fs = FsHelper::get_instance();
    const auto path = cwd.is_absolute() ? cwd : ((fs.m_map["id"]->cwd()) / cwd);
    boost::beast::error_code ec;

    auto res = std::make_shared<http::response<http::file_body>>(
        http::status::ok, m_req.version()
    );
    res->set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res->body().open(path.c_str(), boost::beast::file_mode::scan, ec);

    if(ec == boost::beast::errc::no_such_file_or_directory){
        write(http::status::not_found);
        return;
    }

    if(ec){
        write(http::status::internal_server_error);
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
            self->handle_write(ec);
        }
    );
}

void Session::read_file(
    const std::string& cwd,
    const std::string& file_name
){
    std::filesystem::path path = cwd + "/" + file_name;
    boost::beast::error_code ec;
    m_parser->get().body().open(path.c_str(), boost::beast::file_mode::write, ec);

    if(ec){
        std::cout << ec.message() << std::endl;
        return;
    }

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

void Session::execute_string_request(
    std::shared_ptr<http::request_parser<http::string_body>> parser
){
    const auto& req = parser->get();
    auto method = req.method();
    auto target = req.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write(http::status::bad_request);
        return;
    }

    std::cout << target << std::endl;
    std::cout << req.body() << std::endl;
    if(!nlohmann::json::accept(req.body())){
        std::cout << "body 파싱 불가" << std::endl;
        std::cout << req.body() << std::endl;
        write(http::status::bad_request);
        return;
    }

    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
    }

    nlohmann::json json = nlohmann::json::parse(req.body());
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
}

void Session::execute_empty_request(
    std::shared_ptr<http::request_parser<http::empty_body>> parser
){
    const auto& req = parser->get();
    auto method = req.method();
    auto target = req.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write(http::status::bad_request);
        return;
    }

    std::cout << target << std::endl;
    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
    }

    nlohmann::json json = nlohmann::json::parse(req.body());
    if(method == http::verb::get && path == "/ls"){
        if(map.find("id") == map.end()){
            write(http::status::bad_request);
        }
        write(http::status::ok, Service::ls(map["id"]));
    }
    else if(method == http::verb::get && path == "/exist"){
        if(map.find("id") == map.end() || map.find("path") == map.end()){
            write(http::status::bad_request);
        }
        int32_t ret = Service::exist_file(map["id"], map["path"]);
        write(http::status::ok, {{"result", ret}});
    }
}


/*
void Session::execute_request(){
    auto method = m_req.method();
    auto target = m_req.target();
    auto const uri = boost::urls::parse_relative_ref(target);
    if(!uri){
        write(http::status::bad_request);
        return;
    }

    std::cout << target << std::endl;
    std::cout << m_req.body() << std::endl;
    if(method != http::verb::get && !nlohmann::json::accept(m_req.body())){
        std::cout << "파싱 불가" << std::endl;
        std::cout << m_req.body() << std::endl;
        write(http::status::bad_request);
        return;
    }

    auto path = uri->encoded_path();
    std::unordered_map <std::string, std::string> map;
    for(auto const& p : uri->params()){
        map[p.key] = p.value;
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
        if(map.find("id") == map.end()){
            write(http::status::bad_request);
        }
        write(http::status::ok, Service::ls(map["id"]));
    }
    else if(method == http::verb::get && path == "/download"){
        if(map.find("id") == map.end() || map.find("path") == map.end()){
            write(http::status::bad_request);
        }
        write_file(http::status::ok, map["id"], map["path"]);
    }
    else if(method == http::verb::get && path == "/exist"){
        if(map.find("id") == map.end() || map.find("path") == map.end()){
            write(http::status::bad_request);
        }
        int32_t ret = Service::exist_file(map["id"], map["path"]);
        write(http::status::ok, {{"result", ret}});
    }
}
*/