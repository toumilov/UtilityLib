#pragma once

#include "Error.hpp"
#include "Containers/Value.hpp"

namespace UtilityLib {
namespace Serialization {

class Json
{
public:
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

    static bool validate( const std::string &json, Error &e );
//    Containers::Value parse( const std::string &json, Error &e );

private:
    static bool validate_number( const std::string &num, Error &e );
};

} // namespace Serialization
} // namespace UtilityLib
