#pragma once

#include <cstdarg>
#include <string>

namespace UtilityLib {

class Error
{
protected:
    unsigned code_;
    std::string module_;
    std::string description_;

public:
    enum ErrorCodes
    {
        None = 0xFFFFFFFF,
        Fail = 1
    };

    Error();
    Error( const ErrorCodes code );
    template <class T>
    Error( T e ) :
        code_( e ),
        module_( error_module( e ) )
    {}
    template <class T>
    Error( T e, const char *description_fmt, ... ) :
        code_( e ),
        module_( error_module( e ) )
    {
        va_list arguments;
        va_start( arguments, description_fmt );
        unsigned len = vsnprintf( nullptr, 0, description_fmt, arguments ) + 1;
        description_.resize( len );
        vsnprintf( (char*)description_.c_str(), len, description_fmt, arguments );
        va_end( arguments );
    }

    Error& operator=( const Error &e );
    Error& operator<<( const std::string &description );
    unsigned code() const;
    const std::string& description() const;
    const std::string& module() const;
    bool empty() const;
    void clear();
};

const char* error_module( const Error::ErrorCodes& );

} // namespace UtilityLib
