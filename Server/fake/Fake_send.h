// Copyright Siemens
#ifndef UNITTESTS_FAKE_SEND_H
#define UNITTESTS_FAKE_SEND_H
#include "../interface/Interface_send.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class Fake_send : public Interface_send {
 public:
    char copy_buf[1024];
    //char *buf;
    size_t len;
    ssize_t Send(int sockfd, const void *buf, size_t len, int flags);
};
#endif