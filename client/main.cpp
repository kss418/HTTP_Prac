#include <iostream>
#include <string>
#include <thread>
#include "include/CmdHelper.hpp"
#include "include/FsHelper.hpp"
#include "include/Session.hpp"

int main(){
    boost::asio::io_context io_context;
    auto work = boost::asio::make_work_guard(io_context);
    std::thread th([&](){
        io_context.run();
    });


    CmdHelper cmd_helper(io_context);
    while(1){
        cmd_helper.get_cmd();
    }

    work.reset();
    th.join();
    return 0;
}