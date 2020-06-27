#include <string>

class User
{
    
private:
    std::string username;
    std::string current_channel;
 
    int socket;
public:
    User();
    ~User();
    void setUsername(std::string &name);
    std::string getUsername() const;

    void setCurrent_Channel(std::string channel);
    std::string getCurrent_Channel() const;

    void setSocket(const int sock);
    int  getSocket() const;



};