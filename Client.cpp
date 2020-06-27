#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h> 
#include <string.h>
#include <thread>
#define PORT 8080
const char RETURN=10;


void listen_server(int sockfd)
{   

    
    char buff[1024];
    bzero(buff, sizeof(buff));
    int bytesReceived;

    while(true)
    {  
        bytesReceived = recv(sockfd, buff, 1024,0);  // receive a message from the server
        if (bytesReceived == -1)
        {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }
        else if (bytesReceived == 0)
        {

            std::cout << "You are disconnected from the server.....";
            close(sockfd);
            return;
        }
        else
        {
            std::cout << buff << std::endl;
            bzero(buff, sizeof(buff));
        }
        
        
    }

}




void func(int sockfd) 
{ 
    char buff[1024]; 
    int n;

    //Create the listening thread
    std::thread receive_thread(listen_server,sockfd);


    //send messages from the main thread    
    while(true) 
    {   
        bzero(buff, sizeof(buff)); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n'); //introd. the message from the keybord
		if(std::string(buff) =="/exit\n")
		{
			close(sockfd); 
            exit(1);
		}
        send(sockfd, buff, n,0); //send the message to the server
        
    } 
} 




int main() 
{ 
    //Create socket 
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if(server == -1)
    {
        std::cerr << "Can't create socket" << std::endl;
        return -1;
    }
    
    //Bind the ip adress and the port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = inet_addr("127.0.0.1");
    hint.sin_port = htons(PORT);
    //Connect to the server
    if(connect(server, (sockaddr*)&hint,sizeof(hint)) != 0)
    {
        std::cerr << "Connection with the server failed" << std::endl;
    }
    else
    {
        std::cout<< "Welcome!" << std::endl;
    }

    func(server); 
  
    // close the socket 
    close(server); 
} 