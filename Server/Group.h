#ifndef EASYCHAT_SERVER_GROUP_H
#define EASYCHAT_SERVER_GROUP_H
#include "Channel.h"
#include <string>

class Group : public Channel {
 public:
    Group();
    ~Group();
    void setTopic(const std::string &topic) override;
    std::string getTopic() const override;

 private:
    std::string topic;
};

#endif
