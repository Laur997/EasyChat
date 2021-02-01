// Copyright Siemens
#ifndef EASYCHAT_SERVER_CHANNEL_H
#define EASYCHAT_SERVER_CHANNEL_H
#include <iostream>
#include <vector>
#include <string>
#include "User.h"
#include "Message.h"
#include <memory>


class Channel {
 public:
    Channel();
    Channel(std::string name);
    std::string getName() const;
    std::string getType() const;
    void setName(const std::string &channel_name);
    void setType(const std::string &channel_type);
    void addUser(const std::shared_ptr<User> &member);
    void addMessage(const Message &msg);
    virtual void setTopic(const std::string &topic);
    virtual std::string getTopic() const;
    void importMessages(const std::vector<Message> &messages);
    void erase(const std::shared_ptr<User> &member);
    void Atributes() const;
    bool is_a_Member(const std::string &name) const;
    std::vector<int> getTargets() const;
    std::vector<std::shared_ptr<User>> getMembers();
    std::string to_string_members() const;
    int getUsersCapacity();
    int getMessagesCapacity();
    std::vector<Message> getMessages();
   

 protected:
    std::string name;
    std::string type;
    std::vector<std::shared_ptr<User>> members;
    std::vector<Message> messages;
    std::vector<int> target_sockets;
    int users_capacity = 20;
    int messages_capacity = 20;
};
#endif
