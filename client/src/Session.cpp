#include "../include/Session.hpp"

Session::Session(
    boost::asio::io_context& io_context,
    const std::string& ip,
    unsigned short port
) : m_endpoint(boost::asio::ip::make_address_v4(ip), port),
    m_socket(io_context)
{
    m_socket.async_connect(
        m_endpoint, 
        [this](const boost::system::error_code& ec){
            handle_connect(ec);
        }
    );
}