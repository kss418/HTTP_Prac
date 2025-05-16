#include "../include/Service.hpp"
#include "../include/DBHelper.hpp"
#include <iostream>

bool Service::login(json body){
    auto& db_helper = DBHelper::get_instance();
    std::string id = body.value("id", "");
    std::string pw = body.value("pw", "");
    
    return db_helper.match_pw(id, pw);
}