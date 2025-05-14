#include <iostream>
#include <filesystem>
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include "include/CmdHelper.hpp"
#include "include/FsHelper.hpp"

namespace http = boost::beast::http;

int main(){
    CmdHelper cmd_helper;
    const std::string ip = "127.0.0.1";
    const unsigned short port = 8080;
    const int version = 11;

    boost::asio::io_context io_context;
    boost::beast::tcp_stream stream(io_context);
    boost::asio::ip::tcp::endpoint endpoint = {boost::asio::ip::make_address(ip), port};
    stream.connect(endpoint);

    http::request <http::string_body> req{
        http::verb::get, "login", version
    };
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream, req);

    while(1){
        //cmd_helper.get_cmd();
    }
}