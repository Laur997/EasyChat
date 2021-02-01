// Copyright Siemens
#include "User.h"
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <unistd.h> 

// bool operator==(User & lhs, User & rhs )
// {
//   return lhs.socket == rhs.socket;
// }


User::User() {
  setCurrent_Channel("default");
}

User::~User() {
  close(socket);
  socket = -1;
}

///
/// assigns a value to the "username" member
///
/// @param name
///
void User::setUsername(const std::string &name) {
  this->username = name;
}

///
/// @return the "username" member
///
std::string User::getUsername() const {
  return username;
}

///
/// assigns a value to the "current_channel" member and notifies
/// the client that he/she is now on the group
///
void User::setCurrent_Channel(const std::string &channel) {
  this->current_channel = channel;
}

///
/// @return the "curent_channel" member
///
std::string User::getCurrent_Channel() const {
  return current_channel;
}

///
/// assigns a value to the "socket" member
///
/// @param sock - the client socket
void User::setSocket(const int sock) {
  this->socket = sock;
}

///
/// @return the "socket" member
///
int  User::getSocket() const {
  return socket;
}
