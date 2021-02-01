// Copyright Siemens
#include "../interface/Interface_send.h"
#include "Fake_send.h"
#include "../Server.h"
#include "../User.h"
#include "../Channel.h"
#include <sys/socket.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>
#include <iostream>
#include <cstring>

ssize_t Fake_send::Send(int sockfd, const void *buf, size_t len, int flags) {
        strcpy(this->copy_buf, (char*)buf);
        this->len = len;
        return len;
}
