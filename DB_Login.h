#ifndef EASYCHAT_DB_LOGIN_H
#define EASYCHAT_DB_LOGIN_H
#include <iostream>
#include <string>
#include "Login.h"


class DB_Login : public Login
{
public:  

DB_Login();

bool confirmation(const std::string &username,const std::string &password);

};

#endif