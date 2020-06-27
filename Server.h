#include <map>
#include <vector>
class Server
{
public:
    
    void start();
    static std::vector<User> online_users; //list of logged users


private:    
    void handle_connection(int clientSocket); 
    User login_part(int sockfd,std::string &procedure);
    std::vector<int> get_sockets(std::vector<User> &online_users, int index );
    void send_to_the_others(User &current_user, const std::string &message); 
    int itExist(std::string &username);
    void send_message(User &user, char *buff);
    void execute_command(const int current_socket,const std::string &command);
    void admin_command_line();
    void execute_admin_command(const std::string &commnand);
};