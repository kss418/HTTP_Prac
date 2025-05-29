#include "../include/Session.hpp"

Session::Session(
    boost::asio::io_context& io_context,
    const tcp::endpoint& endpoint
) : m_endpoint(endpoint),
    m_socket(io_context)
{
    m_host = m_endpoint.address().to_string() + ":" + std::to_string(endpoint.port());
}

void Session::write_string(
    const http::verb& method, 
    const std::string& target, const json& json
){
    auto req = std::make_shared<http::request<http::string_body>>(
        method, target, 11
    );
    req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req->set(http::field::host, m_host);
    
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
            handle_write(ec, bytes_transffered);
        }
    );
}

void Session::read(){
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

void Session::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        write();
    }
}

void Session::handle_write(
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

void Session::handle_read(
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