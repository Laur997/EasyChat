// Copyright Siemens

class Client {
 public:
int configure_connection(void);
void listen_server(int sockfd);
void start_chat(int sockfd);
};