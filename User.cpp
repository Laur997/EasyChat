#include <iostream>
#include <string>
#include "User.h"
#include <unistd.h> // for close() function


User::User()
{
  setCurrent_Channel("default");
}

User::~User()
{
  //close(this->getSocket());
  //std::cout << "The socket " << this->getSocket() << " is closed " << std::endl;
  std::cout << "closed" << std::endl;
}

void User::setUsername( std::string &name)
{
  this->username = name;
}

std::string User::getUsername() const
{
  return username;
}

void User::setCurrent_Channel(std::string channel)
{
  this->current_channel = channel;
}

std::string User::getCurrent_Channel() const
{
  return current_channel;
}

void User::setSocket(const int sock)
{
  this->socket = sock;
}

int  User::getSocket() const
{
  return socket;
}

