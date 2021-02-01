#ifndef EASYCHAT_SERVER_MESSAGE_H
#define EASYCHAT_SERVER_MESSAGE_H
#include <string>
#include <ctime>

class Message {
 public:
    Message(const std::string &text, const std::string &sender,
            const std::string &channel);
   Message();
    ~Message();
    
    int getSec();
    int getMin();
    int getHour();
    int getMDay();
    int getWDay();
    int getMonth();
    int getYear();
    std::string getText();
    std::string getSender();
    std::string getChannel();
    void setTime();
    void setSec(int sec);
    void setMin(int min);
    void setHour(int hour);
    void setMDay(int day);
    void setWDay(int day);
    void setMonth(int month);
    void setYear(int year);
    void setText(const std::string &tx);
    void setSender(const std::string &sd);
    void setChannel(const std::string &ch);
    std::string getClockFormat();
    std::string getDate();
 private:
    int sec, min, hour, wday,mday, month, year;
    std::string text, sender, channel;
};

#endif