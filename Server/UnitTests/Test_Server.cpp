#include "Test_Server.h"
#include <cstring>


void Test_Server::setUp() {
}

void Test_Server::tearDown() {
}
//=============================================================================
void Test_Server::setEnviroment() {
    Server server;
    // create the target user (the one who will receive the message)
    User temp_user;
    temp_user.setUsername("Laur");
    temp_user.setSocket(0);
    temp_user.setCurrent_Channel("default");
    Channel ch("default");
    ch.add(temp_user);
    server.c_ls["defalut"] = ch;
    server.online_users.push_back(temp_user);
}
//=============================================================================
void Test_Server::testFilter_privateMessage() {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);
    // "send" a message
    char input[] = "<Laur>Salut";
    server.filter(user, input, fake);
    char *output = "Private message from  admin : Salut";   
    CPPUNIT_ASSERT_EQUAL(strcmp(output, fake.copy_buf), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(fake.len), static_cast<int>(strlen(output)));
}
//=============================================================================
void Test_Server::testFilter_inexistentClient(void) {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);

    // "send" a message
    char input[] = "<Ionut>Salut";
    server.filter(user, input, fake);
    char *output = "This user is not online....";
    CPPUNIT_ASSERT_EQUAL(strcmp(output, fake.copy_buf), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(fake.len),
                         static_cast<int>(strlen(output)));
}
//=============================================================================
void Test_Server::testFilter_LeftBracket() {
    Server server;
    Fake_send fake;
    //setEnviroment();
    User temp_user;
    temp_user.setUsername("Laur");
    temp_user.setSocket(0);
    temp_user.setCurrent_Channel("default");
    Channel ch("default");
    ch.add(temp_user);
    server.c_ls["default"] = ch;
    server.online_users.push_back(temp_user);
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    //server.c_ls["default"].add(user);
    //server.online_users.push_back(user);
    // "send" a message
    char input[] = "<Laur Salut!";
    server.filter(user, input, fake);
    
    char *output = "admin: <Laur Salut!";
    
    CPPUNIT_ASSERT_EQUAL(strcmp(output, fake.copy_buf), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(fake.len), static_cast<int>(strlen(output)));
}
//=============================================================================
void Test_Server::testNullBuffer() {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);

    // "send" a message
    char *input;
    input = NULL;
    CPPUNIT_ASSERT_THROW(server.filter(user, input, fake), std::invalid_argument);
}
//=============================================================================
void Test_Server::testSimpleMessage() {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);

    char input[] = "Salut!";
    server.filter(user, input, fake);
    
    char *output = "admin: Salut!";
    
    CPPUNIT_ASSERT_EQUAL(strcmp(output, fake.copy_buf), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(fake.len), static_cast<int>(strlen(output)));

}
//=============================================================================
void Test_Server::testOutOfRange() {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);

    char input[2048];
    memset(input, '1', 2048);

    CPPUNIT_ASSERT_THROW(server.filter(user, input, fake), std::out_of_range);
}
//=============================================================================
void Test_Server::testForbiddenCharacter() {
    Server server;
    Fake_send fake;
    setEnviroment();
    // create the user we will actually use
    User user;
    user.setUsername("admin");
    user.setSocket(1);
    user.setCurrent_Channel("default");
    server.c_ls["default"].add(user);
    server.online_users.push_back(user);
    char *input = "œžŸ¢¥";  // forbidden character
    CPPUNIT_ASSERT_THROW(server.filter(user, input, fake), std::invalid_argument);
}
//=============================================================================