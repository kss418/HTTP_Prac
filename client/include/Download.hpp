#pragma once
#include <memory>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

class Download : public std::enable_shared_from_this<Download>{
public:
    using tcp = boost::asio::ip::tcp;
    using req = http::request<http::empty_body>;
    using parser = http::response_parser<http::file_body>;
    
    using parser_ptr = std::shared_ptr<parser>;
    
    Download(
        boost::asio::io_context& io_context,
        const std::string& ip,
        unsigned short port,
        http::verb method,
        boost::beast::string_view target
    );
    
    void connect();
    void write();
    void read();
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    const int m_version = 11;
    boost::beast::flat_buffer m_buffer;
    http::verb m_method;
    boost::beast::string_view m_target;
    std::string m_target_buf;
    std::string m_host;
    parser_ptr m_parser;

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