// Copyright Siemens

#ifndef EASYCHAT_SERVER_SERVER_H
#define EASYCHAT_SERVER_SERVER_H
#include "Channel.h"
#include "Group.h"
#include "PrivateChat.h"
#include "Utils.h"
#include "interface/Interface_send.h"
#include "real/Real_send.h"
#include <map>
#include <thread>
#include <memory>
#include <vector>
#include <string>
class Server {
 public:  // all methods are made public to be easier when they are tested
    void start();
    static std::map<std::string, std::shared_ptr<User>> users;
    static std::map<std::string, std::shared_ptr<Channel>> channels; 
    int itExist(const std::string &username);
    std::shared_ptr<User> login_part(int sockfd);
    void handle_connection(int clientSocket);
    bool limit_riched();
    void clear_threads();
    void start_chat(std::shared_ptr<User> &user);
    void send_in_channel(const std::shared_ptr<User> &current_user,
                         std::shared_ptr<Channel> &ch,
                         std::string &message,
                         Interface_send &Test = Real_send::getRealInstance());
    void filter(std::shared_ptr<User> &user, char *buff,
                        Interface_send &Test = Real_send::getRealInstance());
    void send_to_everyone(std::shared_ptr<User> &current_user, const std::string &message);
    void send_to_everyone(const std::string &message);
    void send_package(int sockfd, std::string &channel_name);
    void send_username(const std::shared_ptr<User> user);
    void execute_command(std::shared_ptr<User> &current_user, const std::string &command,
                         Interface_send &Test = Real_send::getRealInstance());
    void admin_command_line();
    void load_channels();
    void load_private_chats();
    void load_users();
    void load_messages();
    void show_channels(int current_socket);
    void show_channels();
    void save_channels();
    void save_private_chats();
    void save_messages();
    void update_channels_for_client();
    void update_users_for_client();
    
};
#endif  // EASYCHAT_SERVER_SERVER_H
