#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "include/DBHelper.hpp"
#include "include/Acceptor.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

void chk(){
    const char* salt_env = std::getenv("SALT");
    if(!salt_env){
        std::cout << "SALT 환경변수 미설정" << std::endl;
    }

    const char* db_pw = std::getenv("DB_PASSWORD");
    if(!db_pw){
        std::cout << "DB_PASSWORD 환경변수 미설정" << std::endl;
    }
}

int main(){
    boost::asio::io_context io_context;
    chk();
    Acceptor acceptor(io_context, 8080);
    io_context.run();

    return 0;
}