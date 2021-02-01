// Copyright Siemens
#ifndef UNITTESTS_REAL_SEND
#define UNITTESTS_REAL_SEND
#include "../interface/Interface_send.h"
#include <sys/socket.h>

class Real_send : public Interface_send {

 private:
   ssize_t Send(int sockfd, const void *buf, size_t len, int flags);

 public:
   static Real_send& getRealInstance();


};
#endif
