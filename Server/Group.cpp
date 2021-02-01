#include "Group.h"


Group::Group() {
    this->users_capacity = 20;
}

Group::~Group() {}


///
/// @return the channel's topic
///
std::string Group::getTopic() const {
    return this->topic;
}

///
///assigns a value to the channel's topic
///
/// @param topic - the topic of the channel 
void Group::setTopic(const std::string &topic) {
    this->topic = topic;
}

