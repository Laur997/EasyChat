// Copyright Siemens
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>
#include "Login.h"
#include "Utils.h"
#include "File_Login.h"

Utils utils;
File_Login::File_Login() {}

///
/// it makes the confirmation utilising files
///
/// @param username
/// @param password
///
///@return confirmation - "true" if the user exists and "false" if it doesn't
bool File_Login::confirmation(const std::string &username,
                              const std::string &password) {
    const char RETURN = 10;
    std::string path = "login_file.txt";
    std::string delimiter = ",";
    bool confirmation = false;
    std::vector<std::string> components;
    std::ifstream log_file;
    log_file.open(path);
    std::string line;
    while (std::getline(log_file, line)) {
           components = utils.split(line, delimiter);
           if (components[0] == username) {
            components[1].erase(components[1].length()-1, 1);
            if (components[1] == password) {
                confirmation = true;
                break;
            }
        }
        }
        log_file.close();
        return confirmation;
}

