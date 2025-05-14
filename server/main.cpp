#include <iostream>
#include <mariadb/conncpp.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include "include/DBHelper.hpp"

namespace ssl = boost::asio::ssl;

int main(){
    DBHelper& db_helper = DBHelper::get_instance();
    

    return 0;
}