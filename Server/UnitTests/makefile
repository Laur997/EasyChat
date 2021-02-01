test: ../interface/Interface_send.o ../fake/Fake_send.o ../real/Real_send.o
	g++ test.cpp Test_Server.o -I.. -I../fake -I../interface -I/usr/include/mysql -L/usr/bin -lcygmysqlclient-18 -pthread -L. -lServer -lcppunit -o test.exe
Test_Server.o: Test_Server.cpp
	g++ -c -I.. -I../fake -I../interface Test_Server.cpp