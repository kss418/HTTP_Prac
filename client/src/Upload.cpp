#include "../include/Upload.hpp"
#include "../include/FsHelper.hpp"
#include <iostream>

Upload::Upload(
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
    m_target_buf = std::string(target);
    m_target = m_target_buf;
    m_req = std::make_shared<req>(m_method, m_target, m_version);
    m_res = std::make_shared<res>();
}

void Upload::connect(){
    m_socket.async_connect(
        m_endpoint, 
        [self = shared_from_this()](const boost::system::error_code& ec){
            self->handle_connect(ec);
        }
    );
}

void Upload::write(){
    boost::beast::error_code ec;
    auto& fs = FsHelper::get_instance();
    std::filesystem::path path = fs.cwd() / m_file_name;
    
    if(ec){
        std::cout << ec.message() << std::endl;
        return;
    }

    m_req->set(http::field::host, m_host);
    m_req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    m_req->body().open(path.c_str(), boost::beast::file_mode::scan, ec);

    if(ec == boost::beast::errc::no_such_file_or_directory){
        std::cout << "대상 파일이 존재하지 않습니다." << std::endl;
        return;
    }

    if(ec){
        std::cout << ec.message() << std::endl;
        return;
    }
    m_req->prepare_payload();

    http::async_write(
        m_socket,
        *m_req,
        [self = shared_from_this()](
            const boost::system::error_code& ec, 
            std::size_t bytes_transffered)
        {
            self->handle_write(ec, bytes_transffered);
        }
    );
}

void Upload::read(){
    http::async_read(
        m_socket, m_buffer, *m_res,
        [self = shared_from_this()](
            const boost::system::error_code& ec,
            std::size_t bytes_transffered
        ){
        self->handle_read(ec, bytes_transffered);
    });
}

void Upload::handle_connect(const boost::system::error_code& ec){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    else{
        write();
    }
}

void Upload::handle_write(
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

void Upload::handle_read(
    const boost::system::error_code& ec, 
    std::size_t byte_transffered
){
    if(ec){
        std::cout << "읽기 실패 : " << ec.message() << std::endl;
    }
    else{
        m_socket.close();
    }
}