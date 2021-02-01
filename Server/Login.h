// Copyright Siemens
#ifndef EASYCHAT_SERVER_LOGIN_H
#define EASYCHAT_SERVER_LOGIN_H
#include <iostream>
#include <string>

class Login {
 public:
    Login();
    ~Login();
    std::string login_process(int sockfd);
    virtual bool confirmation(const std::string &username,
                              const std::string &password) = 0;
};

#endif  // EASYCHAT_SERVER_LOGIN_H
