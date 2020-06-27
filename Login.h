#ifndef EASYCHAT_LOGIN_H
#define EASYCHAT_LOGIN_H
#include <iostream>

class Login
{
public:
    Login();
    ~Login();
    std::string login_process(int sockfd);
    virtual bool confirmation(const std::string &username,const std::string &password) = 0;
};

#endif