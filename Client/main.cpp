// Copyright Siemens
#include "Client.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <thread>

int main() {
    Client client;
    int server = client.configure_connection();
    client.start_chat(server);
    // close the socket
    close(server);
}
