#pragma once
#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>

namespace http = boost::beast::http;

class Session : public std::enable_shared_from_this <Session> {
public:
    using tcp = boost::asio::ip::tcp;
    Session(std::shared_ptr<tcp::socket> socket);
    
    void read_header();
    void read_file(const std::string& path, const std::string& file_name);
    void read_empty();
    void read_string();
private:
    std::shared_ptr<tcp::socket> m_socket;
    boost::beast::flat_buffer m_buffer;
    http::request_parser<http::empty_body> m_req_header;

    void write_string(http::status status, const nlohmann::json& json = {});
    void write_file(const std::filesystem::path& path, const std::string& file_name);

    void execute_string_request(std::shared_ptr<http::request_parser<http::string_body>> parser);
    void execute_empty_request(std::shared_ptr<http::request_parser<http::empty_body>> parser);
    void execute_file_request();
    
    void handle_write(const boost::system::error_code& ec, const std::string& body_type = "string_body");

    void handle_read_header(const boost::system::error_code& ec);

    void handle_read_string(
        const boost::system::error_code& ec, 
        std::shared_ptr<http::request_parser<http::string_body>> parser
    );
    void handle_read_empty(
        const boost::system::error_code& ec,
        std::shared_ptr<http::request_parser<http::empty_body>> parser
    );

    void handle_read_file(
        const boost::system::error_code& ec
    );
};