#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H
#include <iostream>

class Client_Connection
{
public:
    Client_Connection();
    //getters and setters for the sockets
    void setServerSocket(int sock);
    int  getServerSocket();
    // management functions
    void configure_connection();
    std::string listen_server(int sockfd);
    void sendMessage(const std::string &message);
private:
    int server_socket;

};

#endif // CLIENT_CONNECTION_H
