#include "../include/Session.hpp"
#include <iostream>

void Session::handle_read(const boost::system::error_code& ec){
    if(ec){
        std::cout << "읽기 실패 : " << ec.message() << std::endl;
        std::cout << m_req.method() << std::endl;
        std::cout << m_req.target() << std::endl;
    }
    else{
        execute_request();
    }
}

void Session::handle_write(const boost::system::error_code& ec){
    if(ec){
        std::cout << "쓰기 실패 : " << ec.message() << std::endl;
    }
}