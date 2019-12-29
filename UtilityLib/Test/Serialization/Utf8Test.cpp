#include "Serialization/Utf8.hpp"
#include "CppUTest/TestHarness.h"

using namespace UtilityLib::Serialization;

TEST_GROUP(Utf8ValidationGroup)
{
    UtilityLib::Error e;
    void setup()
    {
        e.clear();
    }
    void teardown()
    {
    }
};

TEST(Utf8ValidationGroup, ToAsciiTest)
{
    auto s = Utf8::to_ascii( U"test - текст 10 €", '*' );
    CHECK( e.empty() );
    STRCMP_EQUAL("test - ***** 10 *", s.c_str());

    s = Utf8::to_ascii( U"test - текст 10 €" );
    CHECK( e.empty() );
    STRCMP_EQUAL("test - ????? 10 ?", s.c_str());
}

TEST(Utf8ValidationGroup, SerializationTest)
{
    std::vector<unsigned char> v = { 0x50, 0x72, 0x69, 0x63, 0x65, 0x3A, 0x20, 0x31, 0x30, 0xE2, 0x82, 0xAC };
    auto buf = Utf8::serialize( U"Price: 10€", e );
    CHECK( e.empty() );
    CHECK( buf == v );

    auto s = Utf8::deserialize( v, e );
    CHECK( e.empty() );
    CHECK( s == U"Price: 10€" );
}

TEST(Utf8ValidationGroup, SerializationFailureTest)
{
    std::u32string s = U"?";
    s[0] = 0xFFFFFFFF;
    auto buf = Utf8::serialize( s, e );
    CHECK_FALSE( e.empty() );
    CHECK( e.code() == Utf8::Result::UnsupportedCharacter );

    e.clear();
    std::vector<unsigned char> v = { 0xFF, 0xFF, 0xFF, 0xFF };
    s = Utf8::deserialize( v, e );
    CHECK_FALSE( e.empty() );
    CHECK( e.code() == Utf8::Result::UnsupportedCharacter );

    e.clear();
    v = { 0xE2, 0x82 };
    s = Utf8::deserialize( v, e );
    CHECK_FALSE( e.empty() );
    CHECK( e.code() == Utf8::Result::UnexpectedEnding );
}
