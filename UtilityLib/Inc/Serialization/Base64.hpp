#pragma once

#include <vector>
#include <string>

namespace UtilityLib {
namespace Serialization {

class Base64
{
    static const std::string mapping_;

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
     * @param len input data length
     * @returns Base64-encoded string
     */
    static std::string encode( const char *data, unsigned len );

    /**
     * @brief encode Encodes input binary data into Base64 string.
     * @param data Base64-encoded string
     * @returns Decoded binary data
     */
    static std::vector<char> decode( const std::string &data );
};

} // namespace Serialization
} // namespace UtilityLib
