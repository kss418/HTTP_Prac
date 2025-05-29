#pragma once
#include <mariadb/conncpp.hpp>
#include <string>
#include <mutex>
#include <memory>

class DBHelper{
public:
    static DBHelper& get_instance();
    static void close();
    bool match_pw(const std::string& id, const std::string& pw);
    bool create_id(const std::string& id, const std::string& pw);

    DBHelper() = default;
    DBHelper(const DBHelper&) = delete;
    DBHelper& operator=(const DBHelper&) = delete;
    DBHelper(DBHelper&&) = delete;
    DBHelper& operator=(DBHelper&&) = delete;
private:
    std::unique_ptr<sql::Connection> connection;
    static std::once_flag m_init_flag;
    static std::unique_ptr<DBHelper> m_instance;
};