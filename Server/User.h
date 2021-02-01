// Copyright Siemens
#ifndef EASYCHAT_SERVER_USER_H
#define EASYCHAT_SERVER_USER_H
#include <string>

class User {
 private:

    std::string username; //the name
    std::string current_channel; //the channel where the user is now active
    int socket = -1;
 public:
    // friend bool operator== (User & lhs, User & rhs );
    User();
    ~User();
    void setUsername(const std::string &name);
    std::string getUsername() const;

    void setCurrent_Channel(const std::string &channel);
    std::string getCurrent_Channel() const;

    void setSocket(const int sock);
    int  getSocket() const;
};
#endif  // EASYCHAT_SERVER_USER_H
