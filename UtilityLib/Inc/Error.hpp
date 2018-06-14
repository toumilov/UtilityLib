#pragma once

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
        None = 0,
        Fail
    };

    Error();
    Error( const ErrorCodes code );
    template <class T>
    Error( T e ) :
        code_( e ),
        module_( error_module( e ) )
    {}
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
