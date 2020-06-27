#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <mysql.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <errno.h>
#include "DB_Login.h"


DB_Login::DB_Login()
{

}

bool DB_Login::confirmation(const std::string &username,const std::string &password)
{   

    //std::cout << "This is the derived class function" << std::endl;

    MYSQL *con;
    MYSQL_RES *result;
    MYSQL_ROW row;
    int tries = 3;
	bool confirmation;


    con = mysql_init(NULL);
    if(mysql_real_connect(con, "127.0.0.1", "Laur", "1234", "EasyChat", 3306, NULL, 0) == NULL)
    {
        std::cout << "The authentication failed with the following message:"<<std::endl;;
        std::cout << mysql_error(con) << std::endl;
        exit(1);
    }
    //create statement for query
    std::string statement = "select count(name) from Users where (name = '";
    statement += username;
    statement += "') AND (password = '";
    statement +=password;
    statement += "');";
    //std::cout << statement<<std::endl;
    mysql_query(con, statement.c_str());
        
    //get the result from the query
    result = mysql_store_result(con);
    row = mysql_fetch_row(result);

    confirmation = (row[0][0] == '0')? false : true;

    mysql_close(con); // close connection 

    return confirmation;

}