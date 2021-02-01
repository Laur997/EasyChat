#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>

class Utils
{
public:
    Utils();
    static std::vector<std::string> split(const std::string &s,
                                          const std::string &delimiter);
    static std::vector<std::string> split(const std::string &s,
                                          char delimiter);
    static bool containsOnlyASCII(char *buf, int buf_size);
    int getch();
    static std::string getTime();
};

#endif // UTILS_H
