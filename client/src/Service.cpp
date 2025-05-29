#include "./include/Service.hpp"

void Service::sign_in(const json& json){
    bool ret = json.value("result", 0);
    std::string path = json.value("path", "");
    if(!ret){
        std::cout << "아이디 또는 비밀번호가 일치하지 않습니다." << std::endl;
    }
    else{
        std::cout << "로그인 성공" << std::endl;
        auto& fs = ServerFsHelper::get_instance();
        fs.set_cwd(path);
        fs.set_id(arg[1]);
        logged_in = 1;
    }
}