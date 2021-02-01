#include "DB_Connection.h"
#include <iostream>
#include <mysql.h>

MYSQL* DB_Connection::con = nullptr;

void DB_Connection::StartConnection() {
    con = mysql_init(NULL);
    if (mysql_real_connect(con, "127.0.0.1",
                          "Laur", "1234", "EasyChat", 3306, NULL, 0) == NULL) {
        std::cout << "The authentication:" << std::endl;
        std::cout << mysql_error(con) << std::endl;
        exit(1);
    }
}

DB_Connection& DB_Connection::getIntance() {
    static DB_Connection temp;
    return temp;
}

MYSQL* DB_Connection::getCon() {
    return con;
}

DB_Connection::DB_Connection() {}