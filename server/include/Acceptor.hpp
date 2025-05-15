#pragma once
#include <boost/asio.hpp>

class Acceptor{
public:
    using tcp = boost::asio::ip::tcp;
    Acceptor(boost::asio::io_context& io_context, unsigned short port);
private:
    tcp::acceptor m_acceptor;
    boost::asio::io_context& m_io_context;

    void accept();
    void handle_accept(
        const boost::system::error_code& ec,
        std::shared_ptr<tcp::socket> socket
    );
};