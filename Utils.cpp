#include <vector>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include "Utils.h"






std::vector<std::string> Utils::split(const std::string &s,const std::string &delimiter) //split a string in pieces depending on delimiter
{
    std::vector<std::string> result;
    int poz = 0; //start position for each word
    for(int i = 0; i < s.length(); i++)
    {
	    if((delimiter == s.substr(i,1)) || (i == s.length()-1))
	    {
		    if(i == s.length()-1)
			{
				result.push_back(s.substr(poz,i-poz+1));
			}
			else
			{
				result.push_back(s.substr(poz,i-poz));
				poz = i+1; //position for the next word
			}
	    }
    } 
        
    return result;

}

int Utils::getch() // auxiliary function for getpass 
    {
        int ch;
        struct termios t_old, t_new;

        tcgetattr(STDIN_FILENO, &t_old);
        t_new = t_old;
        t_new.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

        ch = getchar();

        tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
        return ch;
    }
