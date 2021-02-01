#include "utils.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <termios.h>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>

Utils::Utils()
{

}

std::vector<std::string> Utils::split(const std::string &s,
                                      const std::string &delimiter) {
    std::vector<std::string> result;
    std::string temp = "";
    int poz = 0;  // start position for each word
    for(int i = 0; i < s.length();i++){
        if(s.substr(i,1) == delimiter){
            if(!temp.empty()){
                result.push_back(temp);
                temp = "";
            }
        } else {
            temp += s.substr(i,1);
        }
    }
    return result;
}

std::vector<std::string> Utils::split(const std::string &s,
                                      char delimiter) {
    std::vector<std::string> result;
    std::string temp = "";
    for(int i = 0; i < strlen(s.c_str());i++){
        if(s.c_str()[i] == delimiter){
            if(!temp.empty()){
                result.push_back(temp);
                temp = "";
            }
        } else {
            temp +=s.c_str()[i];
        }
    }
    return result;
}

int Utils::getch() {
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

bool Utils::containsOnlyASCII(char *buf, int buf_size) {
    for(int i = 0; i < buf_size; i++) {
        if((buf[i] < 32) || (buf[i] > 126)) {  // buf[i] included in [32,126]
            return false;
        }
    }
    return true;
}

std::string Utils::getTime() {
    time_t now = time(nullptr);
    tm *ltm = localtime(&now);
    std::string temp = "";
    //temp += ltm->tm_year + "\\";
    //temp += ltm->tm_mon + "\\";
    //temp += ltm->tm_mday + "\\";
    if(ltm->tm_hour < 10) {
        temp += "0" + std::to_string(ltm->tm_hour) + ":";
    }
    else {
    temp += std::to_string(ltm->tm_hour) + ":";
    }
    if(ltm->tm_min < 10) {
        temp +="0" + std::to_string(ltm->tm_min);
    } else {
        temp += std::to_string(ltm->tm_min);
    }

    //temp += ltm->tm_sec;
    return temp;
}
