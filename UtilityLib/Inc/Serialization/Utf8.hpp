#pragma once

#include "Error.hpp"
#include <string>
#include <vector>

namespace UtilityLib {
namespace Serialization {

/**
 * @brief UTF8 text processing class.
 */
class Utf8
{
public:
    /**
     * @brief Result of UTF8 processing.
     */
    class Result : public UtilityLib::Error
    {
    public:
        enum ErrorCode
        {
            UnsupportedCharacter = 1,
            UnexpectedEnding
        };
    };

    /**
     * @brief to_ascii Convert wide character string to ASCII string.
     * Wide characters are replaced with specified placeholder.
     * @param utf8 source string.
     * @param placeholder wide character replacement.
     * @return ASCII string.
     */
    static std::string to_ascii( const std::u32string &utf8, const char placeholder = '?' );

    /**
     * @brief serialize Serialize UTF8 string to byte array.
     * @param utf8 source string.
     * @param e Result variable.
     * @return byte array.
     */
    static std::vector<unsigned char> serialize( const std::u32string &utf8, Error &e );

    /**
     * @brief deserialize Deserialize UTF8 string from byte array.
     * @param utf8 byte array.
     * @param e Result variable.
     * @return UTF8 string.
     */
    static std::u32string deserialize( const std::vector<unsigned char> &utf8, Error &e );
};

} // namespace Serialization
} // namespace UtilityLib
