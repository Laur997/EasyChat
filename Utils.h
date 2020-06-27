#ifndef EASYCHAT_UTILS_H
#define EASYCHAT_UTILS_H
#include <vector>
#include <string>


class Utils
{
public:    
    std::vector<std::string> split(const std::string &s,const std::string &delimiter);
    int getch();

};
#endif //EASYCHAT_UTILS_H