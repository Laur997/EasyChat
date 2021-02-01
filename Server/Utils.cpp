// Copyright Siemens
#include "Utils.h"
#include <unistd.h>
#include <stdio.h>
#include<bits/stdc++.h> 
#include <iostream>
#include <termios.h>
#include <vector>
#include <string>
#include <cstring>
#include <ctime>


/// split a string in pieces depending on delimiter
/// @param s - the string which is going to be splitted
/// @param delimiter - it helps the functions to "know" where to make the separation
///
/// @return a vector with all splitted tokens
std::vector<std::string> Utils::split(const std::string &s,
                                      const std::string &delimiter) {
    std::vector<std::string> result;
    int poz = 0;  // start position for each word
    for (int i = 0; i < s.length(); i++) {
        if ((delimiter == s.substr(i, 1)) || (i == s.length()-1)) {
            if (i == s.length()-1) {
                result.push_back(s.substr(poz, i-poz+1));
            } else {
                result.push_back(s.substr(poz, i-poz));
                poz = i+1;  // position for the next word
            }
        }
    }
    return result;
}


std::vector<std::string> Utils::split(const std::string &s,
                                   char delimiter) {
    std::vector<std::string> result;
    //const char *sir = s.c_str();
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

/// auxiliary function for getpass. It takes a character from the keyboard and return it in integer format
///
/// @return a caracter in integer format 
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
    return temp;
}

int Utils::compare_strings(const std::string &left, const std::string &right) {
    std::string _left = left;
    std::string _right = right;
    transform(_left.begin(), _left.end(), _left.begin(), ::tolower);
    transform(_right.begin(), _right.end(), _right.begin(), ::tolower);
    int min_length = _left.length() <= _right.length() ? _left.length() : _right.length();
    for(int i=0; i < min_length; i++) {
        if(_left[i] != _right[i] ){
            if(_left[i] < _right[i]) {
                return -1;
            } else {
                return 1;
            }
        }
    }
    if(_left.length() == _right.length()) {
        return 0;
    } else if(_left.length() < _right.length()) {
        return -1;
    } else {
        return 1;
    }
}