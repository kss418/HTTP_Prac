#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

class Session{
public:
    using tcp = boost::asio::ip::tcp;
    
    Session(
        boost::asio::io_context& io_context,
        const std::string& ip,
        unsigned short port
    );

private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    const int version = 11;

    void handle_connect(const boost::system::error_code& ec);
};