#include <iostream>
#include <mariadb/conncpp.hpp>
#include <expected>
#include <memory>
#include "include/DBHelper.hpp"

int main(){
    DBHelper& db_helper = DBHelper::get_instance();
    
    auto statment = std::unique_ptr<sql::PreparedStatement>(
        db_helper.connection->prepareStatement(
            "INSERT INTO users (id, password, username) VALUES (?, ?, ?)"
        )
    );

    statment->setString(1, "kss418");
    statment->setString(2, "1234");
    statment->setString(3, "kss418");
    statment->execute();

    if(db_helper.connection){
        db_helper.connection->close();
        db_helper.connection.reset();
    }

    return 0;
}