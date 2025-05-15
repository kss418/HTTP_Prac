#include "../include/Session.hpp"
#include <iostream>

void Session::handle_read(const boost::system::error_code& ec){
    if(ec){
        std::cout << "읽기 실패 : " << ec.message() << std::endl;
    }
    else{
        execute_request();
    }
}