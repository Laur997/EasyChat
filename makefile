Server: main_Server.o Server.o Login.o User.o DB_Login.o File_Login.o Utils.o
	g++  -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread main_Server.o Server.o Login.o  User.o DB_Login.o File_Login.o Utils.o -o Server
	
main_Server.o: main_Server.cpp
	g++ -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread -c main_Server.cpp
	
Server.o: Server.cpp Login.o User.o
	g++ -pthread -c  Server.cpp
	
Login.o: Login.cpp
	g++ -c Login.cpp
	
DB_Login.o: DB_Login.cpp
	g++ -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -c DB_Login.cpp
	
File_Login.o: File_Login.cpp
	g++ -c File_Login.cpp

User.o: User.cpp
	g++ -c User.cpp


Utils.o: Utils.cpp
	g++ -c Utils.cpp