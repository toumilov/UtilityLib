#include "Error.hpp"

namespace UtilityLib {

class Error;

const char* error_module( const Error::ErrorCodes& )
{
    return "UtilityLib";
};

Error::Error() :
    code_( ErrorCodes::None )
{}

Error::Error( const ErrorCodes code ) :
    code_( code ),
    module_( error_module( code ) )
{}

Error& Error::operator=( const Error &e )
{
    code_ = e.code_;
    module_ = e.module_;
    description_ = e.description_;
    return *this;
}

Error& Error::operator<<( const std::string &description )
{
    description_ = description;
    return *this;
}

unsigned Error::code() const
{
    return code_;
}

const std::string& Error::description() const
{
    return description_;
}

const std::string& Error::module() const
{
    return module_;
}

bool Error::empty() const
{
    return code_ == ErrorCodes::None;
}

void Error::clear()
{
    code_ = ErrorCodes::None;
    module_.clear();
    description_.clear();
}

} // namespace UtilityLib
