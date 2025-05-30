#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <nlohmann/json.hpp>
#include <future>
#include <iostream>
#include <memory>
#include <variant>

namespace http = boost::beast::http;
using file_parser = std::shared_ptr<http::response_parser<http::file_body>>;
using empty_parser = std::shared_ptr<http::response_parser<http::empty_body>>;
using string_parser = std::shared_ptr<http::response_parser<http::string_body>>;
using var_parser = std::variant<
    file_parser, empty_parser, string_parser, bool
>;

class Session : public std::enable_shared_from_this <Session> {
public:
    using tcp = boost::asio::ip::tcp;
    using json = nlohmann::json;
    
    Session(
        boost::asio::io_context& io_context,
        const std::string& ip,
        unsigned short port
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
        const http::verb& method, const std::string& target,
        const std::filesystem::path& path
    );

    bool connect();

    var_parser read(const std::filesystem::path& path = "");

    void read_string(std::shared_ptr<std::promise<var_parser>> prom);
    void read_empty(std::shared_ptr<std::promise<var_parser>> prom);
    void read_file(
        std::shared_ptr<std::promise<var_parser>> prom,
        const std::filesystem::path& path
    );
private:
    tcp::endpoint m_endpoint;
    boost::beast::tcp_stream m_socket;
    boost::beast::flat_buffer m_buffer;
    std::string m_host;
    empty_parser m_res_header;

    void handle_connect(
        const boost::system::error_code& ec,
        std::promise<boost::system::error_code>& prom
    );
    void handle_write(
        const boost::system::error_code& ec,
        const std::string& body_type
    );

    void handle_read_header(
        const boost::system::error_code& ec,
        std::shared_ptr<std::promise<var_parser>> prom,
        const std::filesystem::path& path = ""
    );
    void handle_read_string(
        const boost::system::error_code& ec,
        string_parser parser,
        std::shared_ptr<std::promise<var_parser>> prom
    );
    void handle_read_file(
        const boost::system::error_code& ec,
        file_parser parser,
        std::shared_ptr<std::promise<var_parser>> prom
    );
    void handle_read_empty(
        const boost::system::error_code& ec,
        empty_parser parser,
        std::shared_ptr<std::promise<var_parser>> prom
    );
};