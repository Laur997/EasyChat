#include "client_connection.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <thread>
#include <QTextStream>
#define PORT 8080
const char RETURN = 10;
const char ip_address[] = "192.168.1.10";
void Client_Connection::setServerSocket(int sock) {
    this->server_socket = sock;
}
int  Client_Connection::getServerSocket() {
    return this->server_socket;
}
Client_Connection::Client_Connection(){}


std::string Client_Connection::listen_server(int sockfd)
{
    char buff[4096];
    bzero(buff, sizeof(buff));
    int bytesReceived;
    std::string message = "";
    while(true)
    {
        // receive a message from the server
        bytesReceived = recv(sockfd, buff, 4096, 0);
        if (bytesReceived == -1)
        {
             std::cerr << "Error in recv(). Quitting" << std::endl;
             break;
        } else if (bytesReceived == 0) {
            std::cout << "You are disconnected from the server.....";
            close(sockfd);
            message = "#DISCONNECTED_TO_THE_SERVER";
            return message;
         } else {
             message.append((char*)(buff)); // add the message in a QString
             break;
         }
    }
    return message;
}

void Client_Connection::configure_connection() {
    // Create socket
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1) {
        std::cerr << "Can't create socket" << std::endl;
        return;
    }
    // Bind the ip adress and the port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = inet_addr(ip_address);
    hint.sin_port = htons(PORT);
    // Connect to the server
    if (connect(server, reinterpret_cast<sockaddr*>(&hint),
                sizeof(hint)) != 0) {
        std::cerr << "Connection with the server failed" << std::endl;
    } else {
        std::cout<< "Welcome!" << std::endl;
    }
    setServerSocket(server);
}
void Client_Connection::sendMessage(const std::string &message) {
    send(server_socket, message.c_str(), strlen(message.c_str()), 0);
}
