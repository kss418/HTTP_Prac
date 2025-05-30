#pragma once
#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <vector>

namespace http = boost::beast::http;
using json = nlohmann::json;

class Session : public std::enable_shared_from_this <Session> {
public:
    using tcp = boost::asio::ip::tcp;
    Session(std::shared_ptr<tcp::socket> socket);
    
    void read_header();
    void read_file(const std::string& full_path);
    void read_empty();
    void read_string();

    void write_string(http::status status, const nlohmann::json& json = {});
    void write_file(http::status status, const std::filesystem::path& full_path);
    void write_empty(http::status status);
private:
    std::shared_ptr<tcp::socket> m_socket;
    boost::beast::flat_buffer m_buffer;
    std::shared_ptr<http::request_parser<http::empty_body>> m_req_header;

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

namespace Service{
    using json = nlohmann::json;

    bool sign_in(const json& json);
    bool sign_up(const json& json);

    bool mkdir(const json& json);
    bool cd(const json& json);
    json ls(const std::string& id);
    int32_t rm(const json& json);
    int32_t rmdir(const json& json);
    void upload(
        const std::string& id, const std::filesystem::path& path,
        std::shared_ptr<Session> self
    );
    void download(
        const std::string& id, const std::filesystem::path& path,
        std::shared_ptr<Session> self
    );

    std::string cwd(const json& json);
};
