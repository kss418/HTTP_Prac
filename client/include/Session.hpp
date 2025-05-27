#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <future>
#include <concepts>
#include <iostream>
#include <memory>

namespace http = boost::beast::http;


template<typename Body>
concept HttpResponseBody = requires {
    typename http::response<Body>; 
};

template<HttpResponseBody Body>
class Session{
public:
    using tcp = boost::asio::ip::tcp;
    using res = http::response<Body>;
    using res_ptr = std::shared_ptr<res>;
    
    Session(
        boost::asio::io_context& io_context,
        const std::string& ip,
        unsigned short port,
        std::promise <res_ptr>& prom,
        http::verb method,
        boost::beast::string_view target,
        nlohmann::json body = {}
    );

    void write();
    void read();
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    const int m_version = 11;
    boost::beast::flat_buffer m_buffer;
    http::verb m_method;
    boost::beast::string_view m_target;
    nlohmann::json m_body;
    std::string m_host;
    std::promise<res_ptr> & m_prom;
    res m_res;

    void handle_connect(const boost::system::error_code& ec);
    void handle_write(
        const boost::system::error_code& ec,
        std::size_t bytes_transffered
    );
    void handle_read(
        const boost::system::error_code& ec,
        std::size_t bytes_transffered
    );
};

template<HttpResponseBody Body>
Session<Body>::Session(
    boost::asio::io_context& io_context,
    const std::string& ip,
    unsigned short port,
    std::promise<res_ptr>& prom,
    http::verb method,
    boost::beast::string_view target,
    nlohmann::json body
) : m_endpoint(boost::asio::ip::make_address_v4(ip), port),
    m_socket(io_context),
    m_method(method),
    m_target(target),
    m_body(body),
    m_prom(prom)
{
    m_host = ip + ":" + std::to_string(port);
    m_socket.async_connect(
        m_endpoint, 
        [this](const boost::system::error_code& ec){
            handle_connect(ec);
        }
    );
}

template<HttpResponseBody Body>
void Session<Body>::write(){
    auto req = std::make_shared<http::request<http::string_body>>(
        m_method, m_target, m_version
    );
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->set(http::field::host, m_host);
    
    if(!m_body.empty()){
        req->body() = m_body.dump();
        req->prepare_payload();
    }

    http::async_write(
        m_socket,
        *req,
        [req, this](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            handle_write(ec, bytes_transffered);
        }
    );
}

template<HttpResponseBody Body>
void Session<Body>::read(){
    http::async_read(
        m_socket,
        m_buffer,
        m_res,
        [this](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
            handle_read(ec, bytes_transffered);
        }
    );
}

template<HttpResponseBody Body>
void Session<Body>::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        write();
    }
}

template<HttpResponseBody Body>
void Session<Body>::handle_write(
    const boost::system::error_code& ec,
    std::size_t bytes_transffered
){
    if(ec){
        std::cout << "쓰기 실패 : " << ec.message() << std::endl;
    }
    else{
        read();
    }
}

template<HttpResponseBody Body>
void Session<Body>::handle_read(
    const boost::system::error_code& ec,
    std::size_t bytes_transffered
){
    if(ec){
        std::cout << "읽기 실패 : " << ec.message() << std::endl;
    }
    else{
        auto m_res_ptr = std::make_shared<res>(std::move(m_res));
        m_prom.set_value(m_res_ptr);
        m_socket.close();
    }
}