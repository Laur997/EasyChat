// Copyright Siemens

#include "Channel.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

Channel::Channel() {}

Channel::Channel(std::string name) {
    this->name = name;
}

void Channel::setTopic(const std::string &topic) {}
std::string Channel::getTopic() const {return "";}

///
/// @return the "name" member
///
std::string Channel::getName() const {
    return name;
}

///
/// assigns a value to the "name" member
///
/// @param name
///
void Channel::setName(const std::string &channel_name) {
    name = channel_name;
}

std::string Channel::getType() const {
    return type;
}

void Channel::setType(const std::string &channel_type) {
    type = channel_type;
}

/// adds a new user in the members list, if it is not already included
/// and if the channel is not full yet
/// @param member - a user object meant to be added in member's list
///
void Channel::addUser(const std::shared_ptr<User> &member) {
    // check if it is enough "space" for the new member
    if(members.size() < getUsersCapacity()) {
        bool flag = false;
        for(auto& mb : members) {
            // check if the member is already in
            if(mb->getUsername() == member->getUsername()) {
                flag = true;
            }
        }
        if(!flag) {
            members.push_back(member);
        }
    }
}

void Channel::addMessage(const Message &msg) {
    if(messages.size() < getMessagesCapacity()) {
        messages.push_back(msg);
    } else {
        messages.erase(messages.begin());
        messages.push_back(msg);
    }
}

///
/// it prints on the screen the name and the members (usernames)
///
void Channel::Atributes() const {
    std::cout << "*" + getName() + ":" << std::endl;

    for (const auto& member : members) {
        std::cout << member->getUsername() << std::endl;
    }
}


///
/// verify if a user it is existent on members list
///
///@param name - the user's name
///
/// @return true if the user is a member of this channels, and false if it doesn't
///
bool Channel::is_a_Member(const std::string &name) const {
    for (const auto& member : members) {
        if (member->getUsername() == name) {
            return true;
        }
    }
    return false;
}

/// erase a user in the members list
///
/// @param member - the user which is going to be erased from the list
///
void Channel::erase(const std::shared_ptr<User> &member) {
    for (int i=0; i < members.size(); ++i) {
        if (members[i]->getSocket() == member->getSocket()) {
            members.erase(members.begin() + i);
        }
    }
}
///
/// @return the sockets numbers for all user from this channel
///
std::vector<int> Channel::getTargets() const {
    std::vector<int> target_sockets;
    for (const auto member : members) {
        if(member->getSocket() != -1) {
            target_sockets.push_back(member->getSocket());
        }
    }
    return target_sockets;
}

std::vector<std::shared_ptr<User>> Channel::getMembers() {
    return this->members;
}

///
/// @return a string which contains a list with all members.
///
/// Format: user1 + \n + user2 + \n + .....
///
std::string Channel::to_string_members() const {
    std::string s = "";
    for (const auto &member : members) {
        s += member->getUsername() + "\n";
    }
    return s;
}

int Channel::getUsersCapacity() {
    return this->users_capacity;
}

int Channel::getMessagesCapacity() {
    return messages_capacity;
}

 void Channel::importMessages(const std::vector<Message> &messages) {
    this->messages = messages;
 }

std::vector<Message> Channel::getMessages() {
    return messages;
}