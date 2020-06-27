#include <iostream>
#include <sys/types.h>
#include <unistd.h> // for close() function
#include <sys/socket.h>
#include <netdb.h> // for NI_MAXHOST, NIMAXSERV, memset and getnameinfo
#include <arpa/inet.h> // for inet_ntop and inet_pton functions
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include "Login.h"
#include "User.h"
#include <mysql.h>
#include "Server.h"
#define PORT 8080 


int main()
{

    std::cout << "Hello!" << std::endl;
    Server server;
    server.start();
 
    return 0;
}