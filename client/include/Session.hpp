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
        unsigned short port,
        http::verb method,
        boost::beast::string_view target,
        boost::beast::string_view body = {}
    );

    void write();
    void read();
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    const int m_version = 11;
    boost::beast::flat_buffer m_buffer;
    http::verb m_method;
    boost::beast::string_view m_target;
    boost::beast::string_view m_body;
    std::string m_host;
    http::response <http::string_body> m_res;

    void handle_connect(const boost::system::error_code& ec);
    void handle_write(
        const boost::system::error_code& ec,
        std::size_t bytes_transffered
    );
    void handle_read(
        const boost::system::error_code& ec,
        std::size_t bytes_transffered
    );
};