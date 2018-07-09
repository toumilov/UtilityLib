
#include "Containers/Variant.hpp"
#include "CppUTest/TestHarness.h"

using namespace UtilityLib;

TEST_GROUP(VariantGroup)
{
    void setup()
    {
    }
    void teardown()
    {
    }
};

TEST(VariantGroup, GenericTypesTest)
{
    Containers::Variant<int, bool, char> v;

    // Uninitialized variable should be empty
    CHECK( v.get<int>() == nullptr );
    CHECK( v.get<bool>() == nullptr );
    CHECK( v.get<char>() == nullptr );

    // int
    v = 123;
    CHECK_EQUAL( 123, *v.get<int>() );
    CHECK( v.get<bool>() == nullptr );
    CHECK( v.get<char>() == nullptr );

    // bool
    v = true;
    CHECK_EQUAL( true, *v.get<bool>() );
    CHECK( v.get<int>() == nullptr );
    CHECK( v.get<char>() == nullptr );

    // char
    v = 'a';
    CHECK_EQUAL( 'a', *v.get<char>() );
    CHECK( v.get<int>() == nullptr );
    CHECK( v.get<bool>() == nullptr );
}

TEST(VariantGroup, StructTest)
{
    struct S
    {
        unsigned &ctr;
        S( unsigned &ctr ) : ctr( ctr ) { this->ctr++; }
        S( const S &s ) : ctr( s.ctr ) { this->ctr++; }
        S( const S &&s ) : ctr( s.ctr ) {}
        ~S() { ctr--; }
    };
    unsigned c = 0;
    {
        Containers::Variant<S> v;
        v = S( c );
        CHECK_EQUAL( 1, c );
    }
    CHECK_EQUAL( 0, c );
}

TEST(VariantGroup, MoveTest)
{
    Containers::Variant<int, bool> v1;
    Containers::Variant<int, bool> v2;
    Containers::Variant<int, bool> v3;

    CHECK( v1.empty() );
    CHECK( v2.empty() );

    v1 = true;
    v2 = std::move( v1 );
    CHECK_EQUAL( true, *v2.get<bool>() );

    v3 = 5;
    auto v4 = Containers::Variant<int, bool>( std::move( v3 ) );
    CHECK_EQUAL( 5, *v4.get<int>() );
}

TEST(VariantGroup, VisitorTest)
{
    struct Visitor
    {
        unsigned i = 0;
        void operator()() { i = 1; }
        void operator()( int ) { i = 2; }
        void operator()( bool ) { i = 3; }
        void operator()( char ) { i = 4; }
    } visitor;
    Containers::Variant<int, bool, char> v;

    v.accept( visitor );
    CHECK_EQUAL( 1, visitor.i );

    v = 123;
    v.accept( visitor );
    CHECK_EQUAL( 2, visitor.i );

    v = true;
    v.accept( visitor );
    CHECK_EQUAL( 3, visitor.i );

    v = 'a';
    v.accept( visitor );
    CHECK_EQUAL( 4, visitor.i );
}
