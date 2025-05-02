#include <iostream>
#include <mariadb/conncpp.hpp>
#include <expected>
#include <memory>

int main(){
    // 환경 변수 설정하기
    std::string pw = getenv("DB_PASSWORD");

    try {
        sql::Driver* driver = sql::mariadb::get_driver_instance();

        sql::Properties props({
            {"hostName", "127.0.0.1"},
            {"port", "3306"},
            {"user", "root"},
            {"password", pw},
        });

        auto connection = std::unique_ptr<sql::Connection>(
            driver->connect(props)  
        );

        if(!connection){
            std::cout << "connection is nullptr" << std::endl;
            return 1;
        }

        auto statment = std::unique_ptr<sql::PreparedStatement>(
            connection->prepareStatement(
                "INSERT INTO users (id, password, username) VALUES (?, ?, ?)"
            )
        );

        statment->setString(1, "kss418");
        statment->setString(2, "1234");
        statment->setString(3, "kss418");
        statment->execute();
        
    }
    catch (sql::SQLException& e){
        std::cerr << "Error: " << e.what();
        std::cerr << " (Code=" << e.getErrorCode() << ")" << std::endl;
    }

    return 0;
}