#include "../include/DBHelper.hpp"
#include <iostream>

std::once_flag DBHelper::m_init_flag;
std::unique_ptr<DBHelper> DBHelper::m_instance;

DBHelper& DBHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        std::atexit(&DBHelper::close);
        m_instance = std::make_unique<DBHelper>();
        auto* driver = sql::mariadb::get_driver_instance();

        sql::Properties props({
            {"hostName", "127.0.0.1"},
            {"port", "3306"},
            {"user", "root"},
            {"password", std::getenv("DB_PASSWORD")},
            {"schema", "test"},
            {"sslCa", "/etc/mysql/certs/ca.pem"},
            {"tlsCert", "/etc/mysql/certs/server-cert.pem"},
            {"tlsKey", "/etc/mysql/certs/server-key.pem"}
        });

        m_instance->connection = std::unique_ptr<sql::Connection>(
            driver->connect(props)  
        );

        if(!m_instance->connection){
            std::cout << "DB 연결 실패" << std::endl;
        }
        else{
            std::cout << "DB 연결 성공" << std::endl;
        }
    });

    return *m_instance;
}

void DBHelper::close(){
    if(m_instance->connection){
        m_instance->connection->close();
        m_instance->connection.reset();
    }
}