#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "client_connection.h"
#include <QMainWindow>
#include <QListWidget>
#include <string>
#include <QWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void send();
    void listen();
    void filter(std::string &message);
    void addChannels(std::string &raw_data,const std::string &channel_type);
    void mark_user();
    void joinChannel();
    void joinPrivate();
    void refresh(QListWidget *list);
    void trim_name(std::string &name);
    void handle_notification(std::string &message, QListWidget *list);
    void notify(std::string sender, QListWidget *list);

private slots:
    void on_sendButton_clicked();

    void on_channelsList_itemDoubleClicked(QListWidgetItem *item);

    void on_usersList_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    Client_Connection *client;
};

#endif // MAINWINDOW_H
