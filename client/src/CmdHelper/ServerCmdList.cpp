#include "../../include/CmdHelper.hpp"
#include "../../include/Session.hpp"

void CmdHelper::server_cd(const std::vector<std::string>& arg){

}

void CmdHelper::server_mkdir(const std::vector<std::string>& arg){

}

void CmdHelper::server_rmdir(const std::vector<std::string>& arg){

}

void CmdHelper::server_ls(){
    std::promise <http::response<http::string_body>> prom;
    std::future <http::response<http::string_body>> fut = prom.get_future();
    Session session(
        m_io_context, "127.0.0.1", 8080, 
        prom, http::verb::post, "/ls", {{"id", id}}
    );
}

void CmdHelper::set_download_file(const std::vector<std::string>& arg){

}

void CmdHelper::set_upload_dir(const std::vector<std::string>& arg){

}
