#include <memory>
#include <CppUTest/TestHarness.h>
#include "Error.hpp"

using namespace UtilityLib;


class TestError : public Error
{
public:
    enum ErrorCode
    {
        Overflow = 1,
        InvalidValue
    };
};

const char* error_module( const TestError::ErrorCode& )
{
    return "TestModule";
};

TEST_GROUP(ErrorGroup)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(ErrorGroup, BasicTest)
{
    Error e;
    CHECK( e.empty() );

    e = Error::Fail;
    CHECK( !e.empty() );
    STRCMP_EQUAL( "UtilityLib", e.module().c_str() );

    e.clear();
    CHECK_EQUAL( Error::None, e.code() );
    CHECK( e.description().empty() );
    CHECK( e.module().empty() );

    e << "TestError";
    STRCMP_EQUAL( "TestError", e.description().c_str() );
}

TEST(ErrorGroup, CustomErrorTest)
{
    Error e;
    e = TestError::Overflow;
    CHECK( !e.empty() );
    CHECK_EQUAL( TestError::Overflow, e.code() );
    STRCMP_EQUAL( "TestModule", e.module().c_str() );
    CHECK( e.description().empty() );

    e = TestError::InvalidValue;
    e << "blablabla";
    CHECK( !e.empty() );
    STRCMP_EQUAL( "TestModule", e.module().c_str() );
    STRCMP_EQUAL( "blablabla", e.description().c_str() );
}
