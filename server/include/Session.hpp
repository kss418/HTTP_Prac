#pragma once
#include <memory>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

namespace http = boost::beast::http;

class Session : public std::enable_shared_from_this <Session> {
public:
    using tcp = boost::asio::ip::tcp;
    Session(std::shared_ptr<tcp::socket> socket);
    
    void read();
private:
    std::shared_ptr<tcp::socket> m_socket;
    boost::beast::flat_buffer m_buffer;
    http::request <http::string_body> m_req;

    void write();
    void execute_request();
    
    void handle_read(const boost::system::error_code& ec);
};