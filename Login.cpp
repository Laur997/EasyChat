#include <iostream>
#include <string>
#include <cstring>
#include "Login.h"
#include <unistd.h> 
#include <sys/socket.h>
#include <errno.h>



Login::Login()
{
}
Login::~Login() = default;

std::string Login::login_process(int sockfd)
{
    bool conf;
    int bytesRecv;
    char name[40];
    char password[40];
    int tries = 3;

   while(conf == false)
    {
        char msg[] = "Introduce username:";
        send(sockfd,msg,sizeof(msg),0);
        bytesRecv = recv(sockfd,name,40,0);
        //std::cout <<"bytes received : " << bytesRecv << std::endl;
		if(bytesRecv == -1)
        {
            printf("Oh dear, something went wrong with recv()! %s\n", strerror(errno));
            break;
        } 
		name[bytesRecv-1] = '\0'; //remove line terminator
        char msgp[] = "Introduce password:";
        send(sockfd,msgp,sizeof(msgp),0);
        bytesRecv = recv(sockfd,password,40,0);
        if(bytesRecv == -1)
        {
            printf("Oh dear, something went wrong with recv()! %s\n", strerror(errno));
            break;
        }
		password[bytesRecv-1] = '\0'; //remove line terminator
	
        conf = confirmation(std::string(name), std::string(password));


		if(conf == false)
		{
			tries--;
			if(tries == 0)
			{
				return "(NULL)";
                break;
			}
		}
        
    }

return std::string(name);


}


