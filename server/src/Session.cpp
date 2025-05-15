#include "../include/Session.hpp"
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

void Session::execute_request(){
    auto method = m_req.method();
    auto target = m_req.target();

    auto pos_q = target.find('?');
    std::string path = target.substr(0, pos_q == -1 ? target.size() : pos_q);
    std::string arg = target.substr(pos_q == -1 ? target.size() : pos_q);

    if(method == http::verb::post && path == "/login"){
        login();
    }
}

void Session::login(){
    std::cout << "로그인" << std::endl;
}