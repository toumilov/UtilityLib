#pragma once

#include <vector>
#include <string>

namespace UtilityLib {
namespace Serialization {

class Base64
{
public:
    /**
     * @brief validate Checks whether input string contains valid Base64-encoded data.
     * @param data Base64-encoded string
     * @returns True if input string is valid, or false otherwise.
     */
    static bool validate( const std::string &data );

    /**
     * @brief encode Encodes input binary data into Base64 string.
     * @param data Binary data buffer
     * @param len Input data length
     * @returns Base64-encoded string
     */
    static std::string encode( const char *data, unsigned len );

    /**
     * @brief encode_hex Encodes input hex string into Base64 string.
     * @param data Hex string buffer
     * @param len Hex string length (must be even)
     * @returns Base64-encoded string
     */
    static std::string encode_hex( const char *data, unsigned len );

    /**
     * @brief decode Decodes input Base64 string to binary data.
     * @param data Base64-encoded string
     * @returns Decoded binary data
     */
    static std::vector<char> decode( const std::string &data );

    /**
     * @brief decode_hex Decodes input Base64 string to hex string.
     * @param data Base64-encoded string
     * @returns Decoded hex string
     */
    static std::string decode_hex( const std::string &data );

    /**
     * @brief encoded_size Returns size (in bytes) of Base64-encoded buffer.
     * @param size raw(decoded) data size
     * @returns data size required for Base64-encoded data
     */
    static unsigned encoded_size( unsigned size );

    /**
     * @brief decoded_size Returns size (in bytes) of Base64 decoded buffer.
     * @param encoded Base64-encoded data
     * @returns data size required for decoded Base64 data
     */
    static unsigned decoded_size( const std::string &encoded );
};

} // namespace Serialization
} // namespace UtilityLib
