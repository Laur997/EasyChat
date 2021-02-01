// Copyright Siemens
#ifndef EASYCHAT_SERVER_UTILS_H
#define EASYCHAT_SERVER_UTILS_H
#include <vector>
#include <string>


class Utils {
 public:
    static std::vector<std::string> split(const std::string &s,
                                   const std::string &delimiter);
    static std::vector<std::string> split(const std::string &s,
                                   char delimiter);
    static bool containsOnlyASCII(char *buf, int buf_size);
    static int getch();
    static std::string getTime();
    static int compare_strings(const std::string &left, const std::string &right);
};
#endif  // EASYCHAT_SERVER_UTILS_H
