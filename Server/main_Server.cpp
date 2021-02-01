// Copyright Siemens
/*! \mainpage Easychat Index Page
 *
 * \section intro_sec Introduction
 *
 * EasyChat is a multithread client-server aplication
 *
 * \section install_sec Installation
 * -install - a complete database with users, groups and messages
 * \subsection Instructions:
 * - run 3 cygwin terminal and 
 * - in the first one: run the sql server
 * - in the second one: run the server with "./server" 
 * -in the third one: run XServer with "xinit" command and the "./client" 
 * - Enjoy chatting 
 * 
 */
#include <unistd.h>  // for close() function
#include <sys/types.h>
#include <sys/socket.h>
#include <mysql.h>
#include <arpa/inet.h>  // for inet_ntop and inet_pton functions
#include <string.h>
#include <netdb.h>  // for NI_MAXHOST, NIMAXSERV, memset and getnameinfo
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "Login.h"
#include "User.h"
#include "Server.h"
#define PORT 8080

int main() {
    Server server;
    server.start();
    return 0;
}
