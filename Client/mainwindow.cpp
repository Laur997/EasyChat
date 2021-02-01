#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  "utils.h"
#include <iostream>
#include <thread>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextStream>
#include <QTime>
#include <QFont>

char delimiter = 30;
static std::string user;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    connect(ui->sendButton, &QPushButton::clicked, this, send);
//    connect(ui->channelsList, &QListWidget::itemDoubleClicked, this, joinChannel);
//    connect(ui->usersList, &QListWidget::itemDoubleClicked, this, joinPrivate);
    client = new Client_Connection();
    client->configure_connection();
    std::thread receiving_thread(&MainWindow::listen, this);
    receiving_thread.detach();

}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}


void MainWindow::send() {
    client->sendMessage(ui->textBox->displayText().toStdString());
    ui->textBox->clear();
}


void MainWindow::listen() {
    while (true) {
        std::string message = client->listen_server(client->getServerSocket());
        //chat->addItem(message.c_str());
        filter(message);

  }
}


void MainWindow::filter(std::string &message) {
    if (message != "") {
            if (message[0] != '#') {
                ui->chat->addItem(QString(message.c_str()));
            } else {
                if (message.substr(0,4) == "#cls") {
                    message.erase(0,4);  // get rid of "#cls"
                    addChannels(message, "Group");
                } else if(message.substr(0,4) == "#uls")  {
                    message.erase(0,4);  // get rid of "#uls"
                    addChannels(message, "PrivateChat");
                } else if(message.substr(0,4) ==  "#pkg") {
                    message.erase(0,4);  // get rid of "#pkg"
                    std::vector<std::string> Messages = Utils::split(message, delimiter);
                    ui->chat->clear();
                    for(auto& msg : Messages) {
                       ui->chat->addItem(QString(msg.c_str()));
                    }
                } else if(message.substr(0,4) == "#grp") {
                    handle_notification(message, ui->channelsList);
                } else if(message.substr(0,4) == "#pvc") {
                    handle_notification(message, ui->usersList);
                } else if(message.substr(0,4) == "#unm") {
                    message.erase(0,4);
                    user =  message;
                }
            }

        }
}

void MainWindow::addChannels(std::string &raw_data,const std::string &channel_type) {
    if(ui->channelsList->count() == 0 && ui->usersList->count()) {
        //currentChannel->setText("default");
    }
    QListWidget *list = nullptr;
    //decide which list the elements belong to
    if (channel_type == "Group") {
        list = ui->channelsList;
    } else if (channel_type == "PrivateChat") {
        list = ui->usersList;
        // (homework)print an error message
    }
    //verify if the channel_type is corectly declared
    if(list != nullptr) {
        std::vector <std::string> tokens = Utils::split(raw_data, "\n");
        if(list->count() == 0) {
            for(std::string& token : tokens) {
                list->addItem(QString(token.c_str()));
            }
        } else {
            list->clear();
            for(std::string& token : tokens) {
                list->addItem(QString(token.c_str()));
            }
        }

    }
    if(channel_type == "PrivateChat") {
        mark_user();
    }
}

void MainWindow::mark_user() {
    if(!user.empty()) {
        std::vector<std::string> tokens;
        for(int i = 0; i < ui->usersList->count(); i++) {
            if(ui->usersList->item(i)->text().toStdString().find("-") != std::string::npos) {
                std::string row = ui->usersList->item(i)->text().toStdString();
                tokens = Utils::split(row, "-");
                tokens[0].pop_back();
                //chat->addItem(QString(tokens[0].c_str()));
                if(tokens[0] == user) {
                    tokens[0] += " - you";
                    ui->usersList->item(i)->setText(tokens[0].c_str());
                }
            }
        }
    } else {
        ui->chat->addItem("Empty username"); //(homework) delete this 'else'
    }
}

void MainWindow::joinChannel() {
    if(ui->usersList->selectedItems().size() > 0) {
        ui->usersList->currentItem()->setSelected(false);
    }
    QFont font;
    font.setBold(false);
    ui->channelsList->currentItem()->setFont(font);
    std::string raw_data = ui->channelsList->currentItem()->text().toStdString();
    std::vector<std::string> tokens = Utils::split(raw_data, "-");
    tokens[0].pop_back(); // get rid of the blank space
    //currentChannel->setText(tokens[0].c_str());
    std::string message = "/join " + tokens[0];
    client->sendMessage(message);
}

void MainWindow::joinPrivate() {
    if(ui->channelsList->selectedItems().size() > 0) {
        ui->channelsList->currentItem()->setSelected(false);
    }
    QFont font;
    font.setBold(false);
    ui->usersList->currentItem()->setFont(font);
    std::string name = ui->usersList->currentItem()->text().toStdString();
    trim_name(name);
    //currentChannel->setText(name.c_str());
    std::string message = "/join " + name;
    client->sendMessage(message);
}


void MainWindow::refresh(QListWidget *list) {
    std::vector<QListWidgetItem* > ls;
    for(int i = 0; i < list->count(); i++) {
        ls.push_back(list->item(i)->clone());
    }
    list->clear();
    for(int i = 0; i < ls.size(); i++) {
        list->addItem(ls[i]);
    }
}


void MainWindow::trim_name(std::string &name) {
    if(name.find("-") != std::string::npos) {
        std::vector<std::string> tokens = Utils::split(name, "-");
        tokens[0].pop_back(); // get rid of the blank space
        name = tokens[0];
    }
}


void MainWindow::handle_notification(std::string &message, QListWidget *list) {
    // get rid of the prefix
    message.erase(0,4);
    // construct the name
    std::string name = "";
    int i = 0;
    while(message[i] != '|') {
        name += message[i];
        ++i;
    }
    // verify client's activity status and act properly
    message.erase(0,i+1);
    if(list->selectedItems().size() == 0) {
            notify(name, list);
    } else {
        std::string temp = list->currentItem()->text().toStdString();
        trim_name(temp);
        if(temp == name) {
            ui->chat->addItem(message.c_str());
        } else {
            notify(name, list);
        }
    }
}


void MainWindow::notify(std::string sender, QListWidget *list) {
        for(int i = 0; i < list->count(); ++i) {
            std::string temp = list->item(i)->text().toStdString();
            trim_name(temp);
            if( temp == sender) {
                QFont font = list->item(i)->font();
                font.setBold(true);
                list->item(i)->setFont(font);
            }

        }
        refresh(list);
}

void MainWindow::on_sendButton_clicked()
{
    send();
}

void MainWindow::on_channelsList_itemDoubleClicked(QListWidgetItem *item)
{
    joinChannel();
}

void MainWindow::on_usersList_itemDoubleClicked(QListWidgetItem *item)
{
    joinPrivate();
}
