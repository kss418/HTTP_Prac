#include "../include/DBHelper.hpp"
#include <iostream>
#include <openssl/sha.h>

std::once_flag DBHelper::m_init_flag;
std::unique_ptr<DBHelper> DBHelper::m_instance;

std::string hash(const std::string& pw){
    std::string salt = std::getenv("SALT");
    std::string salted_pw = pw + salt;
    
    unsigned char tmp[SHA512_DIGEST_LENGTH];
    SHA512(
        reinterpret_cast<const unsigned char*>(
            salted_pw.data()
        ), salted_pw.size(), tmp
    );

    std::string ret;
    auto tf = [](unsigned char num) -> char{
        return (num <= 9 ? num + '0' : num + 'A' - 10);
    };

    for(int i = 0;i < SHA512_DIGEST_LENGTH;i++){
        ret.push_back(tf(tmp[i] / 16));
        ret.push_back(tf(tmp[i] % 16));
    }

    return ret;
}   

DBHelper& DBHelper::get_instance(){
    std::call_once(m_init_flag, [](){
        std::atexit(&DBHelper::close);
        m_instance = std::make_unique<DBHelper>();
        std::string pw = std::getenv("DB_PASSWORD");
        
        m_instance->connection = std::make_unique<pqxx::connection>(
           "dbname=test "
            "user=postgres "
            "password=" + pw + " "
            "host=localhost "
            "port=5432"
        );

        if(!(m_instance->connection->is_open())){
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

bool DBHelper::match_pw(const std::string& id, const std::string& pw){
    pqxx::work txn(*connection);
    pqxx::result res = txn.exec("SELECT pw FROM users WHERE id = $1", pqxx::params{id});
    txn.commit();

    if(res.size() == 1){
        std::string stored = res[0][0].as<std::string>();
        return stored == hash(pw);
    }

    return 0;
}

bool DBHelper::create_id(const std::string& id, const std::string& pw){
    if(id.size() > 20){
        return 0;
    }

    pqxx::work txn(*connection);
    pqxx::result res = txn.exec("SELECT * FROM users WHERE id = $1", pqxx::params{id});
    if (!res.empty()) {
        return false;
    }

    txn.exec("INSERT INTO users (id, pw) VALUES ($1, $2)", pqxx::params{id, hash(pw)});
    txn.commit();
    return 1;
}