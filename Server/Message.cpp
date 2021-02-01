#include "Message.h"
#include <iostream>
Message::Message(const std::string &text, const std::string &sender,
            const std::string &channel) {
    this->text = text;
    this->sender = sender;
    this->channel = channel;
    setTime();
}
/// Constructor from Message class
Message::Message() {}
/// Destructor from Message class
Message::~Message() {}

/// store the time when the message is created - year, month, day in week,
/// day in month, hour, minute and second
void Message::setTime() {
    time_t now = time(nullptr);
    tm* send_time = localtime(&now);
    // Format correctly
    this->year = send_time->tm_year + 1900;
    this->month = send_time->tm_mon + 1;
    this->mday = send_time->tm_mday;
    this->wday = send_time->tm_wday;
    this->hour = send_time->tm_hour;
    this->min = send_time->tm_min;
    this->sec = send_time->tm_sec;
}
// Getter functions
int Message::getSec() {
    return this->sec;
}
int Message::getMin() {
    return this->min;
}
int Message::getHour() {
    return this->hour;
}
int Message::getMDay() {
    return this->mday;
}
int Message::getWDay() {
    return this->wday;
}
int Message::getMonth() {
    return this->month;
}
int Message::getYear() {
    return this->year;
}
std::string Message::getText() {
    return this->text;
}
std::string Message::getSender() {
    return this->sender;
}
std::string Message::getChannel() {
    return this->channel;
}

/// Provides the hour when the message was created in a digital format
///
/// @return the string which contains the time in digital format
std::string Message::getClockFormat() {
    std::string temp = "";
    if(this->hour < 10) {
        temp += "0" + std::to_string(this->hour);
    } else {
        temp += std::to_string(this->hour);
    }
    temp += ":";
    if(this->min < 10) {
        temp += "0" + std::to_string(this->min);
    } else {
        temp += std::to_string(this->min);
    }
    return temp;
}

/// Provides the date when the message was created in a standard format
///
/// @return a string which contains the standart formated date
std::string Message::getDate() {
    std::string temp = "";
    temp += std::to_string(this->year) + "-";  // add year
    if(this->month < 10) {
        temp += "0" + std::to_string(this->month);
    } else {
        temp += std::to_string(this->month);
    }
    temp += "-";
    if(this->mday < 10) {
        temp += "0" + std::to_string(this->mday);
    } else {
        temp += std::to_string(this->mday);
    }
    return temp;
}
// Setter functions
void Message::setSec(int sec) {
    this->sec = sec;
}
void Message::setMin(int min) {
    this->min = min;
}
void Message::setHour(int hour) {
    this->hour = hour;
}
void Message::setMDay(int day) {
    this->mday = day;
}
void Message::setWDay(int day) {
    this->wday = day;
}
void Message::setMonth(int month) {
    this->month = month; // MODIFY THIS TO RESOLVE THE BUG
}
void Message::setYear(int year) {
    this->year = year;
}

void Message::setText(const std::string &tx) {
    this->text = tx;
}
void Message::setSender(const std::string &sd) {
    this->sender = sd;
}
void Message::setChannel(const std::string &ch) {
    this->channel = ch;
}



