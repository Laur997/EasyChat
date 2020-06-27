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
#include "DB_Login.h"
#include "File_Login.h"
#include "User.h"
#include "Server.h"
#include <memory>
#define PORT 8080 

std::vector<User> Server::online_users;

static std::string procedure = "";




void Server::start()
{

    //choosing the authantification way (from file or database)
    while( (procedure != "database") && (procedure != "file") )
    {
        std::cout << "Introduce the authentication method - 'file' or 'database' :" << std::endl;
        std::cin >> procedure;
    } 
    //std::cout << "For this session, the authentification will be made with " << procedure << " method"<< std::endl;

    //Make the thread for command line

    std::thread command_line(admin_command_line,this);



    //show_channels();



    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        exit(1);
    }
 
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    //this socket is for listening
    listen(listening, SOMAXCONN);
    std::vector<std::thread> thread_list;
    while(true)
    {
        // Wait for a connection
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        char host[NI_MAXHOST];      // Client's remote name
        char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);
 
        if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            //std::cout << host << " connected on port " << service << std::endl;
            std::cout << "One new user us trying to connect" << std::endl;
        }
        else
        {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
        }
 

        //create new thread for the new connection
        thread_list.push_back(std::thread (handle_connection,this,clientSocket));
        //std::cout << "Number of threads : " << thread_list.size() << std::endl; //VERIFICARE NR THREAD-URI


    }
    for(auto &thread : thread_list)
    {
        thread.join();
    }
 
}


///HANDLE_CONNECTION Function
void Server::handle_connection(int clientSocket) // the argument is a void pointer because of pthread requirement 
{ 
    //Autentification part   
    User user = login_part(clientSocket,procedure);
    std::cout << "Canalul implicit pentru acest user este " << user.getCurrent_Channel() << "!" << std::endl;
    if(user.getUsername() == "(NULL)")
    {
        //user.~User();
        return;
    }
    else
    {
        std::string message;
        char buf[1024];

        // While loop: accept and echo message back to client    
    
        while (true)
        {
            memset(buf, '\0', 1024);
 
            // Wait for client to send data
            int bytesReceived = recv(clientSocket, buf, 1024, 0);

            if (bytesReceived == -1)
            {
                std::cerr << "Error in recv(). Quitting" << std::endl;
                break;
            }
 
            if (bytesReceived == 0)
            {
                std::cout << "Client disconnected " << std::endl;
                std::string discon_message;
            

            
                for(int i=0; i < online_users.size();i++)
                {
                    if(online_users[i].getSocket() == clientSocket)
                    {    
                        //Notify the other clients that this client is disconnected
                        discon_message = online_users[i].getUsername() + " is disconnected.......";
                        send_to_the_others(user , discon_message);
                        //Erase the disconnected client from the logged in users list
                        online_users.erase(online_users.begin() + i);
                    }
                }
                user.~User();
                break;
            }

            buf[bytesReceived-1] = '\0';

            send_message(user, buf);
        

        }
    }
    
}


User Server::login_part(int sockfd, std::string &procedure)
{
    std::string username;
    std::unique_ptr<Login> auth;
    
    if(procedure == "database")
    {
        auth = std::make_unique<DB_Login>();

    }
    else if(procedure == "file")
    {
        auth = std::make_unique<File_Login>();

    }
    else
    {
        std::cout << "This introduced an incorrect statement" << std::endl;
    }
    

    username = auth->login_process(sockfd);

    
    
    while(itExist(username) != -1)
    {
        char msg[] = "This user is already connected.....";
        send(sockfd, msg, sizeof(msg),0);
        sleep(1);
        username = auth->login_process(sockfd);
    }

    //Initiate user

    User user;
    user.setUsername(username); // our user will take the received username

    if(user.getUsername() != "(NULL)")
    {
    user.setSocket(sockfd);
    

    //std::cout << "The current channel for this user: " << user.getCurrent_Channel() << std::endl;
    Server::online_users.push_back(user); //add to the list of the logged users


    char msg[] = "You are now online....";
    send(sockfd,msg,sizeof(msg),0);
    std::string message = user.getUsername() + " is now online";
    send_to_the_others(user,  message);
    }
    

    //delete auth;

    return user;

}

void Server::send_to_the_others(User &current_user,const std::string &message)   // varianta veche : int current_socket,const std::string &message)
{
    char buf[1024];    
    strcpy(buf, message.c_str()); // convert to char array

    for(auto& user : online_users) //int i=0; i < online_users.size(); i++
        {
            if(user.getSocket() != current_user.getSocket())
            {
                send(user.getSocket(), buf, 1024, 0);
            }
        }

    // for(int sock : ch.getTargets())
    // {
    //     if(sock != current_user.getSocket())
    //     {
    //         send(sock, buf, sizeof(buf),0);
    //     }
    // }


}


void Server::send_message( User &user, char *buff)  
{   
   int i = 1;
    int sockfd;
    std::string message;
    if(buff[0] == '<')
    {
        
        std::string username ="";
        while((buff[i] != '>') && i != sizeof(buff))
        {
            username += buff[i];
            i++;
        }

        

        if(buff[i] == '>')
        {
            sockfd = itExist(username);
            if( sockfd != -1)
            {
                message = std::string(buff);
                message.erase(0,i+1);
                message = "Private message from  " + user.getUsername() + " : " + message;
                send(sockfd, message.c_str(), 1024, 0); //be careful with the size
            }
            else
            {
                char msg[] = "This user is not online....";
                send(user.getSocket(), msg,sizeof(msg),0);
            }
        }
        else
        {
       
            // Echo message back to client
            message = user.getUsername()  + ": " + std::string(buff); // Add "Name: " + to the message 

            send_to_the_others(user, message);
        }
    }
    else if (buff[0] == '/' )
    { 

        execute_command(user.getSocket() , std::string(buff));


    }
    
    else
    {
        //std::cout << buff << std::endl;
       
        // Echo message back to client
        message = user.getUsername()  + ": " + std::string(buff); // Add "Name: " + to the message 

        send_to_the_others(user, message);
    }
}


int Server::itExist(std::string &username)
{
    for(User user : online_users)
    {
        if(username == user.getUsername())
        {
            return user.getSocket();
        }
    }
    return -1;
}


void Server::execute_command(const int current_socket,const std::string &command)
{
    if(command == "/ls")
    {
        //send(current_socket,"The list with the other online users:", sizeof("The list with the other online users:"),0);
        sleep(1);
        std::string temp = "Online users:\n";
        for(auto& user : online_users)
        {
           if(user.getSocket() != current_socket)
           {

               temp += user.getUsername() + "\n";
               
           }
        }
        send(current_socket, temp.c_str(),temp.length(),0); 
        //std::cout << temp.c_str() <<  std::endl;
    }
    else
    {
        char msg[] = "This command doesn't exist";
        send(current_socket, msg , sizeof(msg),0);
        std::cout << "The commnand is : " << command << "*" <<std::endl;
    }
    
}

//the commands loop
void Server::admin_command_line()
{
    std::string command;
    std::cout << "Command line activated..." << std::endl;
    while(true)
    {
        std::cin >> command;
        if(command.substr(0,1) == "/")
        {
            execute_admin_command(command);
        }
        else
        {
            std::cout << "This command doesn't exist";
        }
        
        
    }
}

// This functions analize the commands
void Server::execute_admin_command(const std::string &commnand) 
{
    if(commnand.find(" ") != -1)
    {
        // std::vector<std::string> tokens = split(commnand, " ");
        // for(auto t : tokens)
        // {
        //     std::cout << t << std::endl;
        // }

        std::cout << "Nimic";

    }
    else
    {
        if(commnand == "/ls")
        {
            for(auto& user : online_users)
            {
                std::cout << user.getUsername() << std::endl;
            }
        }
        else if(commnand == "/exit")
        {
            for(auto& user : online_users)
            {
                char msg[] = "The server is turned off. You are now disconnected. Have a nice day!";
                send(user.getSocket(), msg , sizeof(msg),0);
                close(user.getSocket());
                user.~User();
            }
            exit(1);
        }
        else
        {
            std::cout << "This command doesn't exist." << std::endl;
        }
    }
    
}

