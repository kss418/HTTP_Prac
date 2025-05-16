#include "../include/Session.hpp"
#include <iostream>

void Session::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        std::cout << "서버 연결 성공" << std::endl;
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
        std::cout << m_res << std::endl;
        m_socket.close();
    }
}