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

class Session : public std::enable_shared_from_this <Session> {
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
    void write_empty(
        const http::verb& method, 
        const std::string& target
    );
    void write_file(
        const http::verb& method, 
        const std::string& target
    );

    bool connect();

    void read_header();
    void read_string();
    void read_empty();
    void read_file(
        const std::string& path, 
        const std::string& file_name
    );
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    boost::beast::flat_buffer m_buffer;
    std::string m_host;
    std::shared_ptr<http::request_parser<http::empty_body>> m_req_header;

    void handle_connect(
        const boost::system::error_code& ec,
        std::promise<boost::system::error_code>& prom
    );
    void handle_write(
        const boost::system::error_code& ec,
        const std::string& body_type
    );

    void handle_read_header(const boost::system::error_code& ec);
    void handle_read_string(
        const boost::system::error_code& ec,
        std::shared_ptr<http::request_parser<http::string_body>> parser
    );
    void handle_read_file(
        const boost::system::error_code& ec,
        std::shared_ptr<http::request_parser<http::file_body>> parser
    );
    void handle_read_empty(
        const boost::system::error_code& ec,
        std::shared_ptr<http::request_parser<http::empty_body>> parser
    );
};