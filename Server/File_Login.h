// Copyright Siemens
#ifndef EASYCHAT_SERVER_FILE_LOGIN_H
#define EASYCHAT_SERVER_FILE_LOGIN_H
#include <string>

class File_Login : public Login {
 public:
    File_Login();
    bool confirmation(const std::string &username, const std::string &password);
};
#endif
