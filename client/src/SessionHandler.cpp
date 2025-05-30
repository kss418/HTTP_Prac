#include "../include/Session.hpp"

void Session::handle_connect(
    const boost::system::error_code& ec,
    std::promise<boost::system::error_code>& prom
){
    if(ec){
        std::cout << "서버 연결 실패 : " << ec.message() << std::endl;
    }
    prom.set_value(ec);
}

void Session::handle_write(
    const boost::system::error_code& ec,
    const std::string& body_type
){
    if(ec){
        std::cout << body_type << " 쓰기 실패 : " << ec.message() << std::endl;
    }
    else{
        // read();
    }
}

void Session::handle_read_header(
    const boost::system::error_code& ec,
    std::shared_ptr<std::promise<var_parser>> prom,
    const std::filesystem::path& path
){
    if(ec){
        std::cout << "헤더 읽기 실패 : " << ec.message() << std::endl;
    }
    else{
        auto const& res_header = m_res_header->get();
        auto body_type = res_header["X-Body-Type"];
        std::cout << body_type << std::endl;

        if(body_type.empty() || body_type == "string_body"){
            read_string(prom);
        }
        else if(body_type == "empty_body"){
            read_empty(prom);
        }
        else if(body_type == "file_body"){
            read_file(prom, path);
        }
        else{

        }
    }
}

void Session::handle_read_string(
    const boost::system::error_code& ec, string_parser parser,
    std::shared_ptr<std::promise<var_parser>> prom
){
    if(ec){
        std::cout << "string_body 읽기 실패 : " << ec.message() << std::endl;
        prom->set_value(false);
    }
    else{
        prom->set_value(parser);
    }
}

void Session::handle_read_empty(
    const boost::system::error_code& ec, empty_parser parser,
    std::shared_ptr<std::promise<var_parser>> prom
){
    if(ec){
        std::cout << "empty_body 읽기 실패 : " << ec.message() << std::endl;
        prom->set_value(false);
    }
    else{
        prom->set_value(parser);
    }
}


void Session::handle_read_file(
    const boost::system::error_code& ec, file_parser parser,
    std::shared_ptr<std::promise<var_parser>> prom
){
    if(ec){
        std::cout << "file_body 읽기 실패 : " << ec.message() << std::endl;
        prom->set_value(false);
    }
    else{
        prom->set_value(parser);
    }
}
