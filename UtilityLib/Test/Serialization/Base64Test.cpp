#include <CppUTest/TestHarness.h>
#include "Serialization/Base64.hpp"

TEST_GROUP(Base64Suite)
{
    void setup()
    {
    }
    void teardown()
    {
    }

    std::string decode_str( const std::string encoded )
    {
        auto bytes = UtilityLib::Serialization::Base64::decode( encoded );
        return std::string( bytes.begin(), bytes.end() );
    }
};

TEST(Base64Suite, Encode)
{
    std::string input( "Test string" );
    auto b64 = UtilityLib::Serialization::Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( b64.c_str(), "VGVzdCBzdHJpbmc=" );

    input = "NoPadding";
    b64 = UtilityLib::Serialization::Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( b64.c_str(), "Tm9QYWRkaW5n" );

    input = "With Paddings";
    b64 = UtilityLib::Serialization::Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( b64.c_str(), "V2l0aCBQYWRkaW5ncw==" );

    input = "";
    b64 = UtilityLib::Serialization::Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( b64.c_str(), "" );
}

TEST(Base64Suite, Decode)
{

    STRCMP_EQUAL( "Test string", decode_str( "VGVzdCBzdHJpbmc=" ).c_str() );
    STRCMP_EQUAL( "NoPadding", decode_str( "Tm9QYWRkaW5n" ).c_str() );
    STRCMP_EQUAL( "With Paddings", decode_str( "V2l0aCBQYWRkaW5ncw==" ).c_str() );
    CHECK( UtilityLib::Serialization::Base64::decode( "" ).empty() );
}

TEST(Base64Suite, Combined)
{
    std::string input = R"(Hello
        World)";
    auto b64 = UtilityLib::Serialization::Base64::encode( input.c_str(), input.length() );
    CHECK( input == UtilityLib::Serialization::Base64::decode( b64 ).data() );
}

TEST(Base64Suite, Validate)
{
    CHECK( UtilityLib::Serialization::Base64::validate( "VGVzdCBzdHJpbmc=" ) );
    CHECK( UtilityLib::Serialization::Base64::validate( "ABC=" ) );
    CHECK( UtilityLib::Serialization::Base64::validate( "AB==" ) );
    CHECK( !UtilityLib::Serialization::Base64::validate( "ABC" ) );
    CHECK( !UtilityLib::Serialization::Base64::validate( "AB=C" ) );
}
