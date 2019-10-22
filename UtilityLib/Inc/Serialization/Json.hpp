#pragma once

#include "Error.hpp"
#include "Containers/Value.hpp"

namespace UtilityLib {
namespace Serialization {

/**
 * @brief JSON data format processing class.
 */
class Json
{
public:
    /**
     * @brief Result of JSON processing.
     */
    class Result : public UtilityLib::Error
    {
    public:
        enum ErrorCode
        {
            UnexpectedCharacter = 1,
            UnexpectedEnding,
            UnexpectedToken,
            BadValue
        };
    };
    /**
     * @brief JSON formatting details.
     */
    struct Format
    {
        char indent_char = ' ';
        unsigned indent_size  = 0;

        Format() :
            indent_char( ' ' ),
            indent_size( 0 )
        {}
        Format( char indent_char, unsigned indent_size ) :
            indent_char( indent_char ),
            indent_size( indent_size )
        {}
        std::string indent( unsigned level ) const;
    };

    /**
     * @brief validate Validates string as JSON.
     * @param json String that contains JSON data.
     * @param e Result variable.
     * @return True if validation successful.
     */
    static bool validate( const std::string &json, Error &e );

    /**
     * @brief parse Parse JSON string.
     * @param json String that contains JSON data.
     * @param e Result variable.
     * @return Variadic value object.
     */
    static Containers::Value parse( const std::string &json, Error &e );

    /**
     * @brief build Build JSON string from value.
     * @param value Data object to build JSON from.
     * @param e Result variable.
     * @return JSON string.
     */
    static std::string build( const Containers::Value &value, Error &e );

    /**
     * @brief build Build JSON string from value using specific formatting.
     * @param value Data object to build JSON from.
     * @param e Result variable.
     * @param formatter Formatting information.
     * @return JSON string.
     */
    static std::string build( const Containers::Value &value, Error &e, const Format formatter );
};

} // namespace Serialization
} // namespace UtilityLib
