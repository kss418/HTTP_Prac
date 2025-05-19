#include "../include/Service.hpp"
#include "../include/DBHelper.hpp"
#include <iostream>

bool Service::sign_in(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");
    
    return db_helper.match_pw(id, pw);
}

int8_t Service::sign_up(json json){
    auto& db_helper = DBHelper::get_instance();
    std::string id = json.value("id", "");
    std::string pw = json.value("pw", "");

    if(id.empty() || pw.empty()){
        return 0;
    }

    
}