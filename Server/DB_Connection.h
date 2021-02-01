#ifndef EASYCHAT_SERVER_DB_CONNECTION_H
#define EASYCHAT_SERVER_DB_CONNECTION_H
#include <mysql.h>

class DB_Connection {
 public:
    static MYSQL *con;
    void StartConnection();
    static DB_Connection& getIntance();
    static MYSQL* getCon();
private:
    DB_Connection();
};
#endif