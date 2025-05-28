#include "../include/Download.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

Download::Download(
    boost::asio::io_context& io_context,
    const std::string& ip,
    unsigned short port,
    http::verb method,
    boost::beast::string_view target,
    const std::string& file_name
) : m_endpoint(boost::asio::ip::make_address_v4(ip), port),
    m_socket(io_context),
    m_method(method),
    m_file_name(file_name)
{    
    m_host = ip + ":" + std::to_string(port);
    m_parser = std::make_shared<parser>();
    m_target_buf = std::string(target);
    m_target = m_target_buf;
}

void Download::connect(){
    m_socket.async_connect(
        m_endpoint, 
        [self = shared_from_this()](const boost::system::error_code& ec){
            self->handle_connect(ec);
        }
    );
}

void Download::write(){
    auto m_req = std::make_shared<req>(m_method, m_target, m_version);
    boost::beast::error_code ec;
    auto& fs = FsHelper::get_instance();
    
    std::filesystem::path path = fs.cwd() / m_file_name;
    m_parser->get().body().open(path.c_str(), boost::beast::file_mode::write, ec);

    if(ec){
        std::cout << ec.message() << std::endl;
        return;
    }

    m_req->set(http::field::host, m_host);
    m_req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    
    std::cout << m_req->target() << std::endl;
    http::async_write(
        m_socket,
        *m_req,
        [m_req, self = shared_from_this()](
            const boost::system::error_code& ec, 
            std::size_t bytes_transffered)
        {
            self->handle_write(ec, bytes_transffered);
        }
    );
}

void Download::read(){
    http::async_read(
        m_socket, m_buffer, *m_parser,
        [self = shared_from_this()](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
        self->handle_read(ec, bytes_transffered);
    });
}

void Download::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        write();
    }
}

void Download::handle_write(
    const boost::system::error_code& ec, 
    std::size_t bytes_transffered
){
    if(ec){
        std::cout << "쓰기 실패 : " << ec.message() << std::endl;
    }
    else{
        read();
    }
}   

void Download::handle_read(
    const boost::system::error_code& ec, 
    std::size_t byte_transffered
){
    if(ec){
        std::cout << "읽기 실패 : " << ec.message() << std::endl;
    }
    else{
        m_parser->get().body().close();
        m_socket.close();
    }
}