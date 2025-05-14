#pragma once
#include <mariadb/conncpp.hpp>
#include <string>
#include <mutex>

class DBHelper{
public:
    static DBHelper& get_instance();
    static void close();

    DBHelper() = default;
    DBHelper(const DBHelper&) = delete;
    DBHelper& operator=(const DBHelper&) = delete;
    DBHelper(DBHelper&&) = delete;
    DBHelper& operator=(DBHelper&&) = delete;
    std::unique_ptr<sql::Connection> connection;
private:
    //std::unique_ptr<sql::Connection> connection;
    static std::once_flag m_init_flag;
    static std::unique_ptr<DBHelper> m_instance;
};