#include "../include/Service.hpp"
#include "../include/DBHelper.hpp"

void Service::login(json body){
    auto& db_helper = DBHelper::get_instance();
    
}