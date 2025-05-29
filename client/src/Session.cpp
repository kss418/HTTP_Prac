#include "../include/Session.hpp"
#include <coroutine>

Session::Session(
    boost::asio::io_context& io_context,
    const tcp::endpoint& endpoint
) : m_endpoint(endpoint),
    m_socket(io_context),
    m_req_header(std::make_shared<http::request_parser<http::empty_body>>())
{
    m_host = m_endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
}

bool Session::connect(){
    std::promise<boost::system::error_code> prom;
    auto fut = prom.get_future();

    m_socket.async_connect(
        m_endpoint, 
        [self = shared_from_this(), &prom](
            const boost::system::error_code& ec
        ){
            self->handle_connect(ec, prom);
        }   
    );

    boost::system::error_code ec = fut.get();
    return !ec;
}

void Session::write_string(
    const http::verb& method, 
    const std::string& target, const json& json
){
    auto connect = std::async(
        std::launch::async, [self = shared_from_this()]{
            return self->connect();
    });

    if(!connect.get()){
        return;
    }

    auto req = std::make_shared<http::request<http::string_body>>(
        method, target, 11
    );
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->set(http::field::host, m_host);
    req->set("X-Body-Type", "string_body");
    
    if(!json.empty()){
        req->body() = json.dump();
        req->prepare_payload();
    }

    http::async_write(
        m_socket,
        *req,
        [req, this](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            handle_write(ec, "string_body");
        }
    );
}

void Session::write_empty(
    const http::verb& method, 
    const std::string& target
){
    auto connect = std::async(
        std::launch::async, [self = shared_from_this()]{
            return self->connect();
    });

    if(!connect.get()){
        return;
    }

    auto req = std::make_shared<http::request<http::empty_body>>(
        method, target, 11
    );
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->set(http::field::host, m_host);
    req->set("X-Body-Type", "empty_body");

    http::async_write(
        m_socket,
        *req,
        [req, this](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            handle_write(ec, "empty_body");
        }
    );
}

/*
void Session::write_file(
    const http::verb& method, 
    const std::string& target
){
    auto connect = std::async(
        std::launch::async, [self = shared_from_this()]{
            return self->connect();
    });

    if(!connect.get()){
        return;
    }

    auto req = std::make_shared<http::request<http::file_body>>(
        method, target, 11
    );
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->set(http::field::host, m_host);
    req->set("X-Body-Type", "file_body");

    http::async_write(
        m_socket,
        *req,
        [req, this](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            handle_write(ec, "file_body");
        }
    );
}
*/

void Session::read_header(){
    http::async_read_header(
        m_socket,
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
        m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec,
            std::size_t bytes_transffereds
        ){
            self->handle_read_string(ec, parser);
        }
    );
}

void Session::read_empty(){
    auto parser = m_req_header;

    http::async_read(
        m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec, std::size_t bytes
        ){
            self->handle_read_empty(ec, parser);
        }
    );
}

void Session::read_file(const std::string& path, const std::string& file_name){
    auto const& header = m_req_header->get();
    auto parser = std::make_shared<http::request_parser<http::file_body>>(std::move(*m_req_header));

    boost::system::error_code ec;
    std::string full_path = path + "/" + file_name;
    parser->get().body().open(
        full_path.c_str(),
        boost::beast::file_mode::write, ec
    );

    if(ec){
        std::cout << "업로드 실패 : " << ec.message() << std::endl;
        return;
    }

    http::async_read(
        m_socket,
        m_buffer,
        *parser,
        [self = shared_from_this(), parser](
            const boost::system::error_code& ec, std::size_t bytes
        ){
            self->handle_read_file(ec, parser);
        }
    );
}