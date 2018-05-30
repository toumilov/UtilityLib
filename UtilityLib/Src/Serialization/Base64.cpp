
#include <algorithm>
#include <cstring>
#include "Serialization/Base64.hpp"

namespace UtilityLib {
namespace Serialization {

const std::string Base64::mapping_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool Base64::validate( const std::string &data )
{
    if ( ( data.length() % 4 ) != 0 ) { return false; }
    for( auto &c : data )
    {
        if ( !( ( c >= 'A' && c <= 'Z' ) ||
                ( c >= 'a' && c <= 'z' ) ||
                ( c >= '0' && c <= '9' ) ||
                c == '+' || c == '/' || c == '=' ) )
        { return false; }
    }
    int num = std::count( data.begin(), data.end(), '=' );
    if ( num > 2 ) { return false; }
    for( ; num > 0; num-- )
    {
        if ( data[data.size() - num] != '=' ) { return false; }
    }
    return true;
}

std::string Base64::encode( const char *data, unsigned len )
{
    std::string result;
    unsigned result_len = ( ( len * 8u ) / 6u ) + ( ( len * 8u ) % 6u ? 1 : 0 );
    unsigned padding_len = ( ( result_len + 3u ) & ~3u ) - result_len;
    result_len = ( result_len + 3u ) & ~3u;
    result.reserve( result_len );
    auto process_chunk = [&result] ( const char *chunk ) {
        result += mapping_[( chunk[0] & 0xfc ) >> 2];
        result += mapping_[( ( chunk[0] & 0x03 ) << 4 ) + ( ( chunk[1] & 0xf0 ) >> 4 )];
        result += mapping_[( ( chunk[1] & 0x0f ) << 2 ) + ( ( chunk[2] & 0xc0 ) >> 6 )];
        result += mapping_[chunk[2] & 0x3f];
    };

    const char *p = data;
    for( ; len > 2; len -= 3, p += 3 )
    {
        process_chunk( p );
    }
    if ( len )
    {
        char leftover[4] = {0};
        for( unsigned i = 0; len > 0; len--, i++, p++ )
        {
            leftover[i] = *p;
        }
        process_chunk( leftover );
        for( unsigned i = 0; i < padding_len; i++ )
        {
            result[result.size() - i - 1] = '=';
        }
    }
    return std::move( result );
}

std::vector<char> Base64::decode( const std::string &data )
{
    std::vector<char> result;
    if ( validate( data ) )
    {
        auto index_by_char = [] ( char c ) -> unsigned {
            if ( c == '=' ) { return 0; }
            auto pos = mapping_.find( c );
            return ( pos == std::string::npos ) ? 0 : pos;
        };
        char buf[4];
        for( unsigned i = 0; i < data.size(); i += 4 )
        {
            buf[0] = index_by_char( data[i] );
            buf[1] = (char)index_by_char( data[i+1] );
            buf[2] = (char)index_by_char( data[i+2] );
            buf[3] = (char)index_by_char( data[i+3] );
            result.push_back( ( buf[0] << 2 ) + ( ( buf[1] & 0x30 ) >> 4 ) );
            result.push_back( ( ( buf[1] & 0x0f ) << 4 ) + ( ( buf[2] & 0x3c ) >> 2 ) );
            result.push_back( ( ( buf[2] & 0x03 ) << 6 ) + ( buf[3] & 0x3f ) );
        }
        auto pos = data.find_last_of( "=" );
        switch( pos )
        {
        case std::string::npos:
            break;
        default:
            result.resize( result.size() - ( data.size() - pos ) );
        }
    }
    return result;
}

} // namespace Serialization
} // namespace UtilityLib
