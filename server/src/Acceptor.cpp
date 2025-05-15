#include "../include/Acceptor.hpp"
#include "../include/Session.hpp"
#include <iostream>

Acceptor::Acceptor(boost::asio::io_context& io_context, unsigned short port) 
  : m_acceptor(io_context, {boost::asio::ip::address_v4::any(), port}),
    m_io_context(io_context)
{	
	m_acceptor.listen();
    accept();	
}

void Acceptor::accept(){
    auto socket = std::make_shared<tcp::socket>(m_io_context);
    m_acceptor.async_accept(
		*socket,
		[this, socket](const boost::system::error_code& ec){
			handle_accept(ec, socket);
		}
	);
}

void Acceptor::handle_accept(
	const boost::system::error_code& ec,
	std::shared_ptr<tcp::socket> socket
){
	if(ec){
		std::cout << "연결 실패 : " << ec.what() << std::endl;
	}
	else{
		auto session = std::make_shared<Session>(std::move(socket));
		session->read();
	}

	accept();
}