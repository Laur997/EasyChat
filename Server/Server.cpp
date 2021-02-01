// Copyright Siemens
#include "Server.h"
#include "Utils.h"
#include "Login.h"
#include "DB_Login.h"
#include "File_Login.h"
#include "User.h"
#include "Channel.h"
#include "PrivateChat.h"
#include "Group.h"
#include "Message.h"
#include "DB_Connection.h"
#include "interface/Interface_send.h"
#include "real/Real_send.h"
#include <sys/types.h>
#include <unistd.h>  // for close() function
#include <sys/socket.h>
#include <netdb.h>  // for NI_MAXHOST, NIMAXSERV, memset and getnameinfo
#include <arpa/inet.h>  // for inet_ntop and inet_pton functions
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <thread>
#include <memory>
#include <mysql.h>
#define PORT 8080
int limit = 950;

/// the list with online users
std::map<std::string, std::shared_ptr<User>> Server::users;

/// the list with available channels
std::map<std::string, std::shared_ptr<Channel>> Server::channels;
std::vector<std::thread::id> trash_threads;
std::vector<std::thread> thread_ls;
static int proc_id = 0;
char delimiter = 30;


/// runs the server
/// load functions are called
/// bind ip adress and port to a socket
void Server::start() {
    std::string procedure = "";
    
    //choosing the authantification way (from file or database)
    while ( proc_id == 0 ) {
        std::cout << "Choose auth. method - file or database :" << std::endl;
        std::cin >> procedure;
        if ( procedure == "database" ) {
            proc_id = 1;
        } else if ( procedure == "file" ) {
            proc_id = 2;
        }
    }
    //  Make the thread for command line
    sleep(0.5);
    DB_Connection DB = DB_Connection::getIntance();
    DB.StartConnection();  // start the connection with the DB
    sleep(0.8);
    std::thread thread_clearer(clear_threads, this);
    std::thread command_line(admin_command_line, this);
    load_users();
    load_channels();
    load_private_chats();
    load_messages();
    //  Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        exit(1);
    }
    //  Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    hint.sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listening, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));

    //  this socket is for listening
    listen(listening, SOMAXCONN);
    
    while ( true ) {
        if(!limit_riched()) {
            //  Wait for a connection
            sockaddr_in client;
            socklen_t clSize = sizeof(client);
            int clientSocket = accept(listening,
                                    reinterpret_cast<sockaddr*>(&client),
                                    &clSize);
            char host[NI_MAXHOST];      //  Client's remote name
            char service[NI_MAXSERV];   //  Service (eg. port)

            memset(host, 0, NI_MAXHOST);
            memset(service, 0, NI_MAXSERV);
            if (getnameinfo(reinterpret_cast<sockaddr*>(&client), sizeof(client),
                            host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
                std::cout << host << " connected on port " << service << std::endl;
            } else {
                inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
                //std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
            }
            //  create new thread for the new connection
            thread_ls.push_back(std::thread(handle_connection,
                                            this, clientSocket));
            //std::cout << "Number of threads : " << thread_ls.size() << std::endl;
        }
    }
    for ( auto &thread : thread_ls ) {
        thread.join();
    }
}


/// calls the login_part function and if it receives
/// a valid client, is added to users.
///After that, it runs the chat part for the client
///
///@param[in] clientSocket -the socket which gives us the username and the password
void Server::handle_connection(int clientSocket) {
    //  Autentification part
    std::shared_ptr<User> current_user = login_part(clientSocket);
    sleep(1); // wait for login page to exit
    if(current_user != nullptr) {
        sleep(1);
        send_username(current_user);
        sleep(1);
        update_channels_for_client();
        sleep(1);
        update_users_for_client();
        sleep(1);
        start_chat(current_user);
        trash_threads.push_back(std::this_thread::get_id());
    } else {
        close(clientSocket);
    }
}


/// It verifies if the limit it's riched
///
///  @return 'false' if the limit is not reached (the server supports at least
///  one more client), and 'true' if the limit is reached
bool Server::limit_riched() {
    if(thread_ls.size() <= limit) {
        return false;
    } else {
        return true;
    }
}

/// It looks up for finished threads and erase them from the list
void Server::clear_threads() {
    int i = 0;
    while(true) {
        sleep(10);
        if(trash_threads.size() > 0) {
            for(int i = 0; i < trash_threads.size(); ++i) {
                for(int j = 0; j < thread_ls.size(); ++j) {
                    if(trash_threads[i] == thread_ls[j].get_id()) {
                        thread_ls[j].join();
                        thread_ls.erase(thread_ls.begin()+j);
                        trash_threads.erase(trash_threads.begin()+i);
                        sleep(1);
                    }
                }
            }
        }
    }
}

/// manages the receive-send process
///
///@param[in] user - the current client

void Server::start_chat(std::shared_ptr<User> &user)
{
    char buf[1024];
    while (true) {
        memset(buf, '\0', 1024);
        // Wait for client to send data
        int bytesReceived = recv(user->getSocket(), buf, 1024, 0);
        if (bytesReceived == -1) {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        } else if (bytesReceived == 0) {
           //std::cout << "Client disconnected " << std::endl;
            std::string discon_msg;
            close(user->getSocket());
            users[user->getUsername()]->setSocket(-1);
            // discon_msg = user->getUsername() + " is disconnected.......";
            // send_to_everyone(user , discon_msg);
            // sleep(1);
            update_channels_for_client();
            sleep(1);
            update_users_for_client();
            break;
            
        } else {
            try{
            filter(user, buf);
            } catch (std::out_of_range &e) {
                std::cout << e.what() << std::endl;
            } catch(std::invalid_argument &e) {
                std::cout << e.what() << std::endl;
            }
        }
    } 

}



/// This function deals with the authentinfication method and
/// creating the client object
///
/// @param[in] sockfd - the current client socket
///
/// @return user object
std::shared_ptr<User> Server::login_part(int sockfd) {
    std::string username;
    std::unique_ptr<Login> auth;
    if (proc_id ==  1) {
        auth = std::make_unique<DB_Login>();
    } else if (proc_id = 2) {
        auth = std::make_unique<File_Login>();
    } else {
        std::cout << "Incorrect statement" << std::endl;
    }

    username = auth->login_process(sockfd);
    while (itExist(username) != -1) {
        char msg[] = "#wrnThis user is already connected.....";
        send(sockfd, msg, sizeof(msg), 0);
        sleep(1);
        username = auth->login_process(sockfd);
    }

    // Initiate user
    if (username != "(NULL)") {
        users[username]->setSocket(sockfd);
        //  add to the list of the logged users
        char msg[] = "#cnfValid";
        send(sockfd, msg, sizeof(msg), 0);
    } else {
        return nullptr;
    }
    return users[username];
}


/// it takes the message received from client and send it to the other users
/// from the same channel
///
/// @param[in] current_user - the current user
/// @param[in] ch - the current channel
/// @param[in] message - the message which is meant to be delivered
///
void Server::send_in_channel(const std::shared_ptr<User> &current_user,
                             std::shared_ptr<Channel> &ch,
                             std::string &message,
                             Interface_send &Test) {
    // make the proper format
    std::string self_msg = message;
    char buf[1080];
    if(ch->getType() == "Group") {
        message = "#grp" + ch->getName() + "|" + message; // group
    } else {
        message = "#pvc" + current_user->getUsername() + "|" + message;
    }
    //  convert to char array
    strcpy(buf, message.c_str());
    for (int sock : ch->getTargets()) {
        if(sock != current_user->getSocket()) {
            send(sock, buf, sizeof(buf), 0);
        } else {
            send(sock, self_msg.c_str(), strlen(self_msg.c_str()),0);
        }
    }
}


/// this function runs from handle_connection
/// send message to all online users
///
/// @param[in] current_user - the current channel
/// @param[in] message - the message which is meant to be delivered
///
void Server::send_to_everyone(std::shared_ptr<User> &current_user, const std::string &message) {
    for (std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
        if (it->second->getSocket() != current_user->getSocket()) {
            send(it->second->getSocket(), message.c_str(), message.length(), 0);
        }
    }
}

/// this function runs from admin command line
/// send message to all online users
///
/// @param[in] message- the message which is meant to be delivered
///
void Server::send_to_everyone(const std::string &message) {
    for (std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
        if(it->second->getSocket() != -1) {
            send(it->second->getSocket(), message.c_str(), message.length(), 0);
        }
    }
}

/// Sends to the client the messages contained in the channel
///
/// @param[in] sockfd - the client's socket
/// @param[in] channel_name -  the name of the channels where the server
/// sends message from
void Server::send_package(int sockfd, std::string &channel_name) {
    std::string package = "#pkg";
    for(auto& msg : channels[channel_name]->getMessages()) {
        package += msg.getClockFormat() + " " +
                   msg.getSender() + ": "+
                   msg.getText()+ delimiter;
    }
    send(sockfd, package.c_str(), package.length(), 0);
}

/// sends the username to the client list
/// @param[in] user - the user
void Server::send_username(const std::shared_ptr<User> user) {
    std::string name = "#unm" + user->getUsername();
    send(user->getSocket(), name.c_str(), strlen(name.c_str()),0);
}

/// decides if a text message is a regular message, a private message or a command
///
///
/// @param user[in] - the current user
/// @param buff[in] - the message
void Server::filter(std::shared_ptr<User> &user, char *buff, Interface_send &Test) {
    int i = 1;
    int sockfd;
    std::string text;
    // Verify if the buffer is null
    if(buff == nullptr) {
        throw std::invalid_argument("null_ptr exception");
    }
    // Verify if the buffer is in out of range
    if(strlen(buff) > 1024) {
        throw std::out_of_range("Out of range");
    }
    // Verify if the buffer is empty
    std::string msg = std::string(buff);
    if(msg.empty()) {
        throw std::invalid_argument("Empty buffer");
    }
    // Verify if there is some forbidden characters
    if(!Utils::containsOnlyASCII(buff, strlen(buff))) {
        std::cout << "Forbidden character found" << std::endl;
        throw std::invalid_argument("The buffer contains forbidden characters!");
    }
    // (homework)IT WOULD BE MORE LEGIBLE WITH A SWITCH-CASE
    if (buff[0] == '<') {
        std::string username = "";
        while ((buff[i] != '>') && buff[i] != '\0') {
            username += buff[i];
            i++;
        }
        if (buff[i] == '>') {
            sockfd = itExist(username);
            if (sockfd != -1) {
                text = std::string(buff);
                text.erase(0, i+1);
                text = Utils::getTime() +  " Private message from  " +
                        user->getUsername() + " : " + text;
                Test.Send(sockfd, text.c_str(), text.length(), 0);
            } else {
                char msg[] = "This user is not online....";
                Test.Send(user->getSocket(), msg, strlen(msg), 0);
            }
        } else {
            // Echo message back to client
            Message message(std::string(buff), user->getUsername(),
                            user->getCurrent_Channel());
            channels[user->getCurrent_Channel()]->addMessage(message);
            text = message.getClockFormat() + " " +
                   message.getSender() + ": " +
                   message.getText();
            send_in_channel(user,
                            channels[user->getCurrent_Channel()],
                            text, Test);
        }
    } else if (buff[0] == '/') {
        execute_command(user, std::string(buff),Test);
    } else {
        // Echo message back to client
            Message message(std::string(buff), user->getUsername(),
                            user->getCurrent_Channel());
            channels[user->getCurrent_Channel()]->addMessage(message);
            text = message.getClockFormat() + " " +
                   message.getSender() + ": "+
                   message.getText();
            send_in_channel(user,
                            channels[user->getCurrent_Channel()],
                            text, Test);
        }
}


/// it verifies if a user is online
///
/// @param[in] username - the name of the user whose name the function must 
/// must verify if it is online or not
/// @return client's socket number if it exists and if it doesn't returns -1
int Server::itExist(const std::string &username) {
    for (std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
        if (username == it->second->getUsername()) {
            return it->second->getSocket();
        }
    }
    return -1;
}

/// analize the command from the client  and execute the proper action
///
/// @param[in] current_user - the current user
/// @param[in] command - the command meant to be executed
///
void Server::execute_command(std::shared_ptr<User> &current_user,
                             const std::string &command,
                             Interface_send &Test) {
    if (command.find(" ") != std::string::npos) {
        std::vector<std::string> tokens = Utils::split(command, " ");
        if (tokens.size() > 1) {
            if (tokens[0] == "/ls" && (tokens[1] == "channels")) {
                show_channels(current_user->getSocket());
            //  if the sintax if correct and the channel actually exists
            } else if (tokens[0] == "/join") {
                //std::cout << command << std::endl;
                if(users.count(tokens[1])) {
                    std::string name;
                    // Order the names alphabetically
                    if(Utils::compare_strings(tokens[1],
                       current_user->getUsername()) == -1) {
                        name = tokens[1] + "-" + current_user->getUsername();
                    } else {
                        name = current_user->getUsername() + "-" + tokens[1];
                    }
                    // Verify if the private chat allready exists
                    if(channels.count(name) == 1) {
                        // join to the existent channel
                        current_user->setCurrent_Channel(name);
                        send_package(current_user->getSocket(), name);
                    // if it doesn't exist, we create it
                    } else {
                        // create a new Private Chat
                        PrivateChat *pc = new PrivateChat();
                        pc->setName(name);
                        pc->addUser(current_user);
                        //std::cout << current_user->getUsername() << " added" << std::endl;
                        pc->addUser(users[tokens[1]]);
                        //std::cout << users[tokens[1]]->getUsername() << " added" << std::endl;
                        pc->setType("PrivateChat");
                        channels[name].reset(pc);
                        current_user->setCurrent_Channel(name);
                        send_package(current_user->getSocket(), name);
                    }
                } else {
                    if(channels.count(tokens[1]) == 1) {
                        current_user->setCurrent_Channel(tokens[1]);
                        send_package(current_user->getSocket(), tokens[1]);
                    }
                }

                
                
            } else {
                char msg[] = "You didn't insert the command correctly";
                Test.Send(current_user->getSocket(), msg, sizeof(msg), 0);
            }
        }
    } else {
        if (command == "/ls") {
            sleep(1);
            std::string temp = "Online users:\n";
            for (std::map<std::string,
                 std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
                if (it->second->getSocket() != current_user->getSocket()) {
                    temp += it->second->getUsername() + "\n";
                }
            }
            Test.Send(current_user->getSocket(), temp.c_str(), temp.length(), 0);
        } else {
            char msg[] = "This command doesn't exist";
            Test.Send(current_user->getSocket(), msg, sizeof(msg), 0);
        }
    }
}

/// analize the command from the administrator  and execute the proper action
///
void Server::admin_command_line() {
    std::string command;
    std::cout << "Command line activated..." << std::endl;
    while (true) {
        std::getline(std::cin, command);
        if (command.find(" ") != std::string::npos) {
            Utils ut;
            std::vector<std::string> tokens = ut.split(command, " ");
            if ( (tokens[0] == "create") && (tokens[1] == "group") ) {
                if (tokens.size() > 2) {
                    std::shared_ptr<Channel> ch = std::make_shared<Group>(); // (homework change to simple pointer ans add with reset)
                    ch->setName(tokens[2]);
                    std::string topic;
                    std::cout << "Give a description about the channel:"
                              << std::endl;
                    std::cin >> topic;
                    ch->setTopic(topic);
                    ch->setType("Group");
                    channels[tokens[2]] = ch;
                    update_channels_for_client();
                    // are some changes so we must mention that for saving step
                } else {
                    std::cout << "You need to specify a Channel name"
                              << std::endl;
                }
            } else if ( (tokens.size() > 2)
                        && (tokens[0] == "change")
                        && (tokens[2] == "topic")) {

                if (channels.count(tokens[1]) == 1) {
                    // Change the topic
                    std::cout << "Topic:" << std::endl;
                    std::string new_topic;
                    std::getline(std::cin, new_topic);
                    channels[tokens[1]]->setTopic(new_topic);
                    std::cout << "Topic changed" << std::endl;
                    // Notify the clients
                    // std::string notification = "!!! The topic of the " +
                    // tokens[1] + " channel has been changed!!!\n"
                    // + "New topic: " + new_topic;
                    // send_to_everyone(notification);
                    // sleep(1);
                    update_channels_for_client();
                } else {
                    std::cout << "There is no such Channel."
                              << std::endl;
                }
            } else if ((tokens[0] == "ls") && (tokens[1] == "channels")) {
                show_channels();
            } else {
                std::cout << "You introduced a wrong command." << std::endl;
            }
        } else {
            if (command == "ls") {
                for (std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
                    std::cout << it->second->getUsername() << std::endl;
                }
            } else if (command == "exit") {
                for (std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
                    char msg[] = "The server is turned off.";
                    send(it->second->getSocket(), msg , sizeof(msg), 0);
                    close(it->second->getSocket());
                }
                // if a new channel was created or a topic was changed
                    std::cout << "Saving..." << std::endl;
                    save_messages();
                    save_channels();
                    save_private_chats();
                    mysql_close(DB_Connection::getCon());
                    exit(1);
            } 
        }
    }
}


/// take group's names and topics, and than create the channels objects
/// and add them on a list
void Server::load_channels() {
    std::string statement;
    MYSQL_RES *result;
    MYSQL_ROW row;
    
    statement = "SELECT name, topic FROM GROUPS;";
    mysql_query(DB_Connection::getCon(), statement.c_str());

    result = mysql_store_result(DB_Connection::getCon());

    int num  = mysql_num_rows(result);

    for (int i = 0; i < num; i++) {
        row = mysql_fetch_row(result);
        Group *ch = new Group();
        ch->setName(std::string(row[0]));
        ch->setTopic(std::string(row[1]));
        ch->setType("Group");
        // include all the users in the group
        for (std::map<std::string, std::shared_ptr<User>>::iterator it=users.begin();
             it != users.end(); ++it) {
            ch->addUser(it->second);
        }
        channels[ch->getName()].reset(ch);
    }
}

/// loads from the database the private-chat channels and store them into
/// 'channels' map 
void Server::load_private_chats() {
    std::string statement;
    MYSQL_RES *result;
    MYSQL_ROW row;
    statement = "SELECT name FROM PrivateChats;";
    mysql_query(DB_Connection::getCon(), statement.c_str());
    result = mysql_store_result(DB_Connection::getCon());
    int num  = mysql_num_rows(result);
    for (int i = 0; i < num; i++) {
        row = mysql_fetch_row(result);
        PrivateChat *pv = new PrivateChat();
        pv->setName(std::string(row[0]));
        pv->setType("PrivateChat");
        // add members
        std::vector<std::string> names = Utils::split(pv->getName(),"-");
        pv->addUser(users[names[0]]);
        pv->addUser(users[names[1]]);
        // add the private chat to the channels;
        channels[pv->getName()].reset(pv);
    }
    
}

/// loads from the database users information and store it into
/// 'users' map 
void Server::load_users() {
    std::string statement;
    MYSQL_RES *result;
    MYSQL_ROW row;
    statement = "SELECT name FROM USERS;";
    //std::cout << "Before query" << std::endl;
    mysql_query(DB_Connection::getCon(), statement.c_str());
    result = mysql_store_result(DB_Connection::getCon());
    //std::cout << "Before query" << std::endl;
    int num  = mysql_num_rows(result);
    for (int i = 0; i < num; i++) {
        row = mysql_fetch_row(result);
        User *user = new User();
        user->setUsername(std::string(row[0]));
        users[user->getUsername()].reset(user);
    }
}

/// load the messages from database and store them into the channels objects
void Server::load_messages() {
    MYSQL_RES *result;
    MYSQL_ROW row;
    std::string statement;

    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
        it != channels.end(); ++it) {
        statement = "SELECT * FROM MESSAGES WHERE channel = '" +
                    it->first +
                    "' ORDER BY send_date, hour, min, sec;";
        mysql_query(DB_Connection::getCon(), statement.c_str());
        result = mysql_store_result(DB_Connection::getCon());
        int number = mysql_num_rows(result);
        for(int i = 0; i < number; i++) {
            row = mysql_fetch_row(result);
            Message message;
            // Add date
            std::string data = std::string(row[0]);
            std::vector<std::string> date = Utils::split(data, "-");
            message.setYear(std::stoi(date[0]));
            // Verify if the month begins with '0'. Not convenient for int type
            for (int i = 0; i < date.size(); i++) {
                if (date[i][0] == '0') {
                date[i].erase(0,1);
                }
            }
            message.setMonth(std::stoi(date[1]));
            // Verify if the day begins with '0'. Not convenient for int type
            message.setMDay(std::stoi(date[2]));
            // Add sender
            message.setSender(std::string(row[1]));
            // Add channel
            message.setChannel(std::string(row[2]));
            // Add text
            message.setText(std::string(row[3]));
            // Add hour
            message.setHour(std::stoi(row[4]));
            // Add minute
            message.setMin(std::stoi(row[5]));
            // Add sec
            message.setSec(std::stoi(row[6]));
            // add message in channel
            it->second->addMessage(message);
            
        }
    }
}


/// print message which contains the channels and there members
/// for the server console
void Server::show_channels() {

    std::string msg = "";
    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
    it != channels.end(); ++it) {
           msg += "\n" + it->first;
    }
}



/// sends to the client a message which contains the available channels and their members
///
///@param[in] current_socket - the socket which is associated with the current client
void Server::show_channels(int current_socket) {   // for the client
    std::string msg = "";
    // iteration in the map
    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
        it != channels.end(); ++it) {
        // if the channel is not empty
        if (it->second->getTargets().size() > 0) {
           msg += "^" +  it->first + " - " +
                  it->second->getTopic() + "\n";
       } else {
           msg +="^" + it->first + " - " + it->second->getTopic() + "\n";
       }
    }
    send(current_socket, msg.c_str(), msg.length(), 0);
}

///
/// saves the content of channels in the database
///
void Server::save_channels() {
    std::string statement;
    statement = "TRUNCATE TABLE channels;"; // truncate table
    mysql_query(DB_Connection::getCon(), statement.c_str());
    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
         it != channels.end(); ++it) {
             if(it->second->getType() == "Group") {
                 statement.clear();
                statement = "INSERT INTO channels(name,topic) VALUES('";
                statement.append(it->second->getName());
                statement.append("','");
                statement.append(it->second->getTopic());
                statement.append("');");
                mysql_query(DB_Connection::getCon(), statement.c_str());
             }
    }
}

/// save the private-chat channels information in the database 
void Server::save_private_chats() {
    std::string statement;
    statement = "TRUNCATE TABLE PrivateChats;"; // truncate table
    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
         it != channels.end(); ++it) {
             if(it->second->getType() == "PrivateChat") {
                 statement.clear();
                statement = "INSERT INTO PrivateChats(name) VALUES('";
                statement.append(it->second->getName());
                statement.append("');");
                mysql_query(DB_Connection::getCon(), statement.c_str());
             }
    }
}

/// send to the user the new status of the channels
void Server::update_channels_for_client() {
    std::string msg = "";
    // iteration in the map
    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
        it != channels.end(); ++it) {
        if(it->second->getType() == "Group") {
            // if the channel is not empty
            if (channels[it->first]->getTargets().size() > 0 &&
                channels[it->first]->getType() == "Group") {
                msg +=  it->first + " - " +
                channels[it->first]->getTopic() + "\n";
            } else {
                msg += it->first + " - " + channels[it->first]->getTopic() + "\n";
            }
        }
    }
    msg = "#cls" + msg;
    send_to_everyone(msg);
}

/// send to the user the new status of the users
void Server::update_users_for_client() {
    std::string msg = "";
    for(std::map<std::string, std::shared_ptr<User>>::iterator it = users.begin();
                 it != users.end(); ++it) {
        if(it->second->getSocket() != -1) {
            msg += it->second->getUsername()+ " - online" + "\n";
        } else {
            msg += it->second->getUsername() + "\n";
        }
    }
    msg = "#uls" + msg;
    send_to_everyone(msg);
}

/// save all the messages in the database
void Server::save_messages() {
    // Delete the old table, if it exists.
    std::string statement = "TRUNCATE TABLE MESSAGES;"; // truncate table
    mysql_query(DB_Connection::getCon(), statement.c_str());

    for (std::map<std::string, std::shared_ptr<Channel>>::iterator it=channels.begin();
        it != channels.end(); ++it) {  // take every channel;
            // take every message from this channels
            std::vector<Message> messages = it->second->getMessages();
            for( auto& msg : messages) {
                statement = "INSERT INTO MESSAGES(send_date, sender, channel,";
                statement += "content, hour, min, sec) VALUES(";
                statement += "'" + msg.getDate() + "', ";
                statement += "'" + msg.getSender() + "', ";
                statement += "'" + msg.getChannel() + "', ";
                statement += "'" + msg.getText() + "', ";
                statement += std::to_string(msg.getHour()) + ",";
                statement += std::to_string(msg.getMin()) + ",";
                statement += std::to_string(msg.getSec()) + ");";
                mysql_query(DB_Connection::getCon(), statement.c_str());
            }
    }
}
