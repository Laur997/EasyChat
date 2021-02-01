Server: main_Server.o Server.o Login.o User.o DB_Login.o File_Login.o Group.o Channel.o Utils.o interface/Interface_send.o real/Real_send.o Message.o DB_Connection.o
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread -o Server.exe main_Server.o Server.o Login.o  User.o DB_Login.o Group.o File_Login.o Channel.o Utils.o interface/Interface_send.o real/Real_send.o Message.o DB_Connection.o
	
main_Server.o: main_Server.cpp
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread -c main_Server.cpp
	
Server.o: Server.cpp Login.o User.o interface/Interface_send.o real/Real_send.o DB_Connection.o
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread -c  Server.cpp
	
Login.o: Login.cpp
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -c Login.cpp
	
DB_Login.o: DB_Login.cpp
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -c DB_Login.cpp
	
File_Login.o: File_Login.cpp
	g++ -g -c File_Login.cpp

User.o: User.cpp
	g++ -g -c User.cpp

Channel.o: Channel.cpp
	g++ -g -c Channel.cpp

Utils.o: Utils.cpp
	g++ -g -c Utils.cpp

Group.o: Group.cpp
	g++ -g -c Group.cpp
PrivateChat.o: PrivateChat.cpp
	g++ -g -c PrivateChat.cpp

Message.o: Message.cpp
	g++ -g -c Message.cpp

DB_Connection.o: DB_Connection.cpp
	g++ -g -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -c DB_Connection.cpp
