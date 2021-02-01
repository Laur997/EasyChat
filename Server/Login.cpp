// Copyright Siemens
#include "Login.h"
#include "Utils.h"
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>


Login::Login() {}
Login::~Login() = default;

///
/// Manages with the login process. It demands the username and the password
/// from the client and it make the confirmation (though the derivated classes)
///
///@param sockfd - the socket which gives us the username and the password
///
///@return it returns the username if it is valid and "(NULL)" if it is invalid
std::string Login::login_process(int sockfd) {
    bool conf = false;
    int bytesRecv;
    char usr_and_pass[1024];
    int tries = 3;
    std::vector<std::string> login;
    while (conf == false) {
        

    bytesRecv = recv(sockfd, usr_and_pass, 1024, 0);
    if (bytesRecv == -1) {
        //printf("Something went wrong with recv()! %s\n", strerror(errno));
        return "(NULL)";
        break;
    } else if(bytesRecv == 0) {
        //printf("Client disconnected....error: %s\n", strerror(errno));
        return "(NULL)";
        break;
    } else {
        std::string temp = std::string(usr_and_pass);
        login  = Utils::split(temp, "/");
        conf = confirmation(login[0], login[1]);
        std::cout << "Username : " << login[0] << " , password : " << login[1] << " and confirmation : " << conf << std::endl;
        if (conf == false) {
            tries--;
            char msg[] = "#wrnUsername or password is wrong!";
            send(sockfd, msg, sizeof(msg), 0);
            if (tries == 0) {
                return "(NULL)";
                break;
            }
        }
    }
            
        
        
        
    }
return login[0];
}


