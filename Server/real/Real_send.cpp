// Copyright Siemens
#include "Real_send.h"
#include <sys/socket.h>
#include <iostream>

ssize_t Real_send::Send(int sockfd, const void *buf, size_t len, int flags) {
    send(sockfd, buf, len, flags);
    return len;
}


Real_send& Real_send::getRealInstance() {
    static Real_send real;
    return real;
}

