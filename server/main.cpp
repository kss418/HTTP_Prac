#include <iostream>
#include <mariadb/conncpp.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "include/DBHelper.hpp"
#include "include/Acceptor.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

int main(){
    boost::asio::io_context io_context;
    Acceptor acceptor(io_context, 8080);
    io_context.run();

    return 0;
}