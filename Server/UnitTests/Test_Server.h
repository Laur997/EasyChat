#include "Server.h"
#include "Fake_send.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>


class Test_Server : public CppUnit::TestFixture {


CPPUNIT_TEST_SUITE(Test_Server);
CPPUNIT_TEST(testFilter_privateMessage);
CPPUNIT_TEST(testFilter_LeftBracket );
CPPUNIT_TEST(testFilter_inexistentClient);
CPPUNIT_TEST(testSimpleMessage);
CPPUNIT_TEST(testNullBuffer);
CPPUNIT_TEST(testOutOfRange); // empty string + special caract
CPPUNIT_TEST(testForbiddenCharacter);
CPPUNIT_TEST_SUITE_END();
    
 public:
    void setEnviroment();
    void testFilter_privateMessage();
    void testFilter_inexistentClient();
    void testFilter_LeftBracket();
    void testNullBuffer();
    void testSimpleMessage();
    void testOutOfRange();
    void testForbiddenCharacter();
    void setUp();
    void tearDown();

};

CPPUNIT_TEST_SUITE_REGISTRATION( Test_Server );