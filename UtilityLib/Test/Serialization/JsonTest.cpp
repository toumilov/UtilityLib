#include "Serialization/Json.hpp"
#include "CppUTest/TestHarness.h"

using namespace UtilityLib::Serialization;

TEST_GROUP(JsonValidationGroup)
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

TEST(JsonValidationGroup, EmptyStringTest)
{
    // Empty string is valid
    CHECK( Json::validate( "", e ) );
    CHECK( e.empty() );
    CHECK( Json::validate( " ", e ) );
    CHECK( e.empty() );
    CHECK( Json::validate( "\t", e ) );
    CHECK( e.empty() );
    CHECK( Json::validate( "\r", e ) );
    CHECK( e.empty() );
    CHECK( Json::validate( "\n", e ) );
    CHECK( e.empty() );
}

TEST(JsonValidationGroup, LexemeTest)
{
    // "true" lexeme
    CHECK( Json::validate( "true", e ) );
    CHECK( e.empty() );

    // "false" lexeme
    CHECK( Json::validate( "false", e ) );
    CHECK( e.empty() );

    // "null" lexeme
    CHECK( Json::validate( "null", e ) );
    CHECK( e.empty() );

    // Whitespaces
    CHECK( Json::validate( " \ttrue\t ", e ) );
    CHECK( e.empty() );

    // Bad lexeme
    CHECK_FALSE( Json::validate( "None", e ) );
    CHECK( e.code() == Json::Result::UnexpectedToken );
    e.clear();

    CHECK_FALSE( Json::validate( "bad", e ) );
    CHECK( e.code() == Json::Result::UnexpectedToken );
    e.clear();
}

TEST(JsonValidationGroup, StringTest)
{
    CHECK( Json::validate( "\"\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\" \"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"test\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"test 123\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"A_1!!!\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\\"\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\\\\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\/\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\b\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\f\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\n\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\r\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\t\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\u0123\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"\\uDead\"", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "\"Feed 1 \\uDead\" ", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( R"_(" ' \\ \" \/ \b \f \n \r \t ,./*+-`~@!?<>_%$;:()#^[]{}|")_", e ) );
    CHECK( e.empty() );

    CHECK_FALSE( Json::validate( "\"\\ubad\"", e ) );
    CHECK( e.code() == Json::Result::UnexpectedCharacter );
    e.clear();
}

TEST(JsonValidationGroup, NumberTest)
{
    std::string valid_numbers[] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "12", "123",
        "11", "22", "33", "44", "55", "66", "77", "88", "99",
        "-0", "-1", "-2", "-3", "-4", "-5", "-6", "-7", "-8", "-9", "-10",
        "-123", "-11", "-22", "-33", "-44", "-55", "-66", "-77", "-88", "-99",
        "0.0", "0.1", "0.2", "0.3", "0.4", "0.5", "0.6", "0.7", "0.8", "0.9", "1.2", "1.23",
        "1.1", "2.2", "3.3", "4.4", "5.5", "6.6", "7.7", "8.8", "9.9", "-0.0",
        "-1.1", "-2.2", "-3.3", "-4.4", "-5.5", "-6.6", "-7.7", "-8.8", "-9.9", "-10.01",
        "-12.3", "-11.1", "-22.2", "-33.3", "-44.4", "-55.5", "-66.6", "-77.7", "-88.8", "-99.9",
        "0e0", "1e1", "2e2", "3e3", "4e4", "5e5", "6e6", "7e7", "8e8", "9e9",
        "12e+12", "123e-123", "11e11", "22e22", "33e33", "44e44", "55e55", "66e66", "77e77",
        "88e88", "99e99", "-0e0", "-1e1", "-2e2", "-3e3", "-4e4", "-5e5", "-6e6", "-7e7",
        "-8e8", "-9e9", "-10e+2", "-123e-45", "-11e11", "-22e22", "-33e33", "-44e44", "-55e55",
        "-66e66", "-77e77", "-88e88", "-99e99", "0E0", "1E1", "2E2", "3E3", "4E4", "5E5",
        "6E6", "7E7", "8E8", "9E9", "12E+12", "123E-123", "11E11", "22E22", "33E33", "44E44",
        "55E55", "66E66", "77E77", "88E88", "99E99", "-0E0", "-1E1", "-2E2", "-3E3", "-4E4",
        "-5E5", "-6E6", "-7E7", "-8E8", "-9E9", "-10E+2", "-123E-45", "-11E11", "-22E22",
        "-33E33", "-44E44", "-55E55", "-66E66", "-77E77", "-88E88", "-99E99"
    };
    std::string invalid_numbers[] = { "-", "00", "01", "0-", "-00", "-012", "-e0", "-E0", "-E12", "e2E4", "e+", "E+", "0e+", "0E+" };

    for( const auto &i : valid_numbers )
    {
        CHECK( Json::validate( i, e ) );
        CHECK( e.empty() );
    }
    for( const auto &i : invalid_numbers )
    {
        CHECK_FALSE( Json::validate( i, e ) );
        CHECK( !e.empty() );
        e.clear();
    }
}

TEST(JsonValidationGroup, ArrayTest)
{
    CHECK( Json::validate( "[]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[ ]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[\t]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[\r\n]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[\"test\"]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[\"test\",5,-123,true]", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "[ \"test\" , 5 , -123 , true ]", e ) );
    CHECK( e.empty() );

    // Missing comma
    CHECK_FALSE( Json::validate( "[ \"test\" , 5 true ]", e ) );
    CHECK( !e.empty() );
}

TEST(JsonValidationGroup, ObjectTest)
{
    CHECK( Json::validate( "{}", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "{ \t}", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( " { \"key\" : \"value\" } ", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "{ \"key\" : \"value\", \"count\" : 5, \"price\" : 1.50 }", e ) );
    CHECK( e.empty() );

    CHECK( Json::validate( "{\"key\":\"value\",\"count\":5,\"price\":1.50,\r\n\"valid\":true,\"promo_code\":null}", e ) );
    CHECK( e.empty() );

    CHECK_FALSE( Json::validate( "{,}", e ) );
    CHECK( !e.empty() );
}

TEST(JsonValidationGroup, CombinedTest)
{
    std::string str = R"_({
    "string": "test",
    "number": 123,
    "object": {
        "key": "value",
        "bool": true,
        "undef": null
    },
    "array": [
        123,
        false,
        "test"
    ],
    "float": 1.5
})_";
    CHECK( Json::validate( str, e ) );
    CHECK( e.empty() );
}

TEST(JsonValidationGroup, InvalidInputTest)
{
    std::string scenarios[] = { "123 {}", "123{}", "123 true", "true {}", "tru", "{", "}", "[", "]" };
    for( const auto &i : scenarios )
    {
        CHECK_FALSE( Json::validate( i, e ) );
        CHECK( !e.empty() );
        e.clear();
    }
}
