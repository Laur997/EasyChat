// Copyright Siemens
#ifndef UNITTESTS_INTERFACE_SEND
#define UNITTESTS_INTERFACE_SEND

#include <sys/socket.h>

class Interface_send {
 public:
   virtual ssize_t Send(int sockfd, const void *buf,
                        size_t len, int flags) = 0;
};

#endif