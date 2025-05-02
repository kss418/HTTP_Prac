#include <iostream>
#include <mariadb/conncpp.hpp>

int main(){
    sql::Driver* driver = sql::mariadb::get_driver_instance();

    // 환경 변수 설정하기
    std::string pw = getenv("DB_PASSWORD");

    sql::Properties props({
        {"host", "localhost"},
        {"port", "3306"},
        {"user", "root"},
        {"password", pw},
        {"schema", "test"} 
    });

    return 0;
}