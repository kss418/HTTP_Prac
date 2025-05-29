#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <future>
#include <iostream>
#include <memory>

namespace http = boost::beast::http;

class Session{
public:
    using tcp = boost::asio::ip::tcp;
    using json = nlohmann::json;
    
    Session(
        boost::asio::io_context& io_context,
        const tcp::endpoint& endpoint
    );
    
    void write_string(
        const http::verb& method, 
        const std::string& target, const json& json
    );
    void write_empty();
    void write_file();

    void read_header();

    void read_string();
    void read_empty();
    void read_file();
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    boost::beast::flat_buffer m_buffer;
    std::string m_host;

    void handle_connect(const boost::system::error_code& ec);
    void handle_write(
        const boost::system::error_code& ec,
        std::size_t bytes
    );
    void handle_read(
        const boost::system::error_code& ec,
        std::size_t bytes
    );
};