#include <iostream>
#include <mariadb/conncpp.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include "include/DBHelper.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

int main(){
    // DBHelper& db_helper = DBHelper::get_instance();
    boost::asio::io_context io_context;
    tcp::acceptor acceptor{io_context, {boost::asio::ip::address_v4::any(), 8080}};

    while(1){
        tcp::socket socket(io_context);
        acceptor.accept(socket);

        boost::beast::flat_buffer buffer;
        http::request <http::string_body> req;
        boost::beast::error_code ec;
        http::read(socket, buffer, req, ec);

        if(ec == boost::beast::http::error::end_of_stream){

        }
        else if(ec){
            std::cout << ec.message() << std::endl;
        }

        std::cout << req << std::endl;


        http::response <http::string_body> res{
            http::status::ok, req.version()
        };
        res.set(http::field::content_type, "text/plain; charset=utf-8");
        res.body() = "로그인 페이지입니다.";
        res.prepare_payload();
        http::write(socket, res);
    }

    return 0;
}