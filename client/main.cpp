#include <iostream>
#include <string>
#include "include/CmdHelper.hpp"
#include "include/FsHelper.hpp"
#include "include/Session.hpp"

namespace http = boost::beast::http;

int main(){
    CmdHelper cmd_helper;

    boost::asio::io_context io_context;
    //Session session(io_context, "127.0.0.1", 8080);
    io_context.run();

    while(1){
        //cmd_helper.get_cmd();
    }
}