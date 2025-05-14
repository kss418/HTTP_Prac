#include "../include/Session.hpp"
#include <iostream>

void Session::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        std::cout << "서버 연결 성공" << std::endl;
    }
}