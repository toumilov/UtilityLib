
#include <cstdio>
#include <stack>
#include <utility>
#include <regex>
#include "Serialization/Json.hpp"


namespace UtilityLib {
namespace Serialization {

const char* error_module( const Json::Result::ErrorCode& )
{
    return "Json";
};

class JsonTokenizer
{
public:
    JsonTokenizer( const std::string &json_str ) :
        str_( json_str ),
        it_( json_str.begin() )
    {
    }

    bool is_end() const
    {
        return it_ == str_.end();
    }

    std::pair<unsigned, unsigned> get_last_token_position()
    {
        return std::make_pair( token_line_, token_offset_ );
    }

    std::string get_token( Error &e )
    {
        token_line_ = line_;
        token_offset_ = offset_;
        std::string token;
        std::string hex_esc;
        while(true)
        {
            if ( is_end() )
            {
                if ( !token.empty() )
                {
                    state_ = State::Undefined;
                }
                return token;
            }
            char c = *it_;
            switch( state_ )
            {
            case State::Undefined:
                if ( isspace( c ) ) // Skip whitespaces
                {
                    advance();
                    continue;
                }
                token += c;
                if ( c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',' )
                {
                    advance();
                    return token;
                }
                else if ( c== '\"' )
                {
                    state_ = State::String;
                }
                else if ( c == '-' || isdigit( c ) )
                {
                    state_ = State::Integer;
                }
                else if ( isalpha( c ) )
                {
                    state_ = State::Lexeme;
                }
                else
                {
                    e = Error( Json::Result::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
                    it_ = str_.end();
                    return "";
                }
                break;
            case State::String:
                token += c;
                if ( c == '\"' )
                {
                    state_ = State::Undefined;
                    advance();
                    return token;
                }
                if ( c == '\\' )
                {
                    state_ = State::Esc;
                }
                break;
            case State::Esc:
                token += c;
                if ( c == '\"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' )
                {
                    state_ = State::String;
                }
                else if ( c == 'u' )
                {
                    state_ = State::EscHex;
                }
                else
                {
                    e = Error( Json::Result::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
                    it_ = str_.end();
                    return "";
                }
                break;
            case State::EscHex:
                token += c;
                if ( isdigit( c ) || ( c >= 'a' && c <= 'f' ) || ( c >= 'A' && c <= 'F' ) )
                {
                    hex_esc += c;
                    if ( hex_esc.length() == 4 )
                    {
                        hex_esc.clear();
                        state_ = State::String;
                    }
                }
                else
                {
                    e = Error( Json::Result::UnexpectedCharacter, "Unexpected character (%d:%d)", line_, offset_ );
                    it_ = str_.end();
                    return "";
                }
                break;
            case State::Integer:
                if ( isdigit( c ) || c == '+' || c == '-' || c == 'e' || c == 'E' || c == '.' )
                {
                    token += c;
                }
                else
                {
                    state_ = State::Undefined;
                    return token;
                }
                break;
            case State::Lexeme:
                if ( isalpha( c ) )
                {
                    token += c;
                }
                else
                {
                    state_ = State::Undefined;
                    return token;
                }
                break;
            }
            advance();
        }
    }

private:
    enum class State
    {
        Undefined = 0,
        String,
        Esc,
        EscHex,
        Integer,
        Lexeme
    } state_ = State::Undefined;
    const std::string &str_;
    std::string::const_iterator it_;
    unsigned line_ = 1;
    unsigned offset_ = 1;
    unsigned token_line_ = 1;
    unsigned token_offset_ = 1;

    void advance()
    {
        if ( is_end() )
        {
            return;
        }
        it_++;
        if ( *it_ == '\n' )
        {
            line_++;
            offset_ = 0;
        }
        else
        {
            offset_++;
        }
    }
};

class JsonImpl
{
    enum class State
    {
        Value,
        Key,
        KeyValueSeparator,
        ValueSeparator,
        End
    };
    enum class Levels
    {
        Object,
        Array
    };

    static bool is_lexeme( const std::string &token )
    {
        return token == "true" || token == "false" || token == "null";
    }

    static bool is_number( const std::string &token )
    {
        return std::regex_match( token, std::regex( R"_(^[-]?(0|[1-9]\d*)(\.\d+)?((e|E)(\+|-)?\d+)?$)_" ) );
    }

    static bool is_string( const std::string &token )
    {
        return token.length() >= 2 && std::regex_match( token, std::regex( R"_(^\"([^\\]*|\\(u[\da-fA-F]{4}|"|\\|\/|b|f|n|r|t))*\"$)_" ) );
    }

public:
    static bool validate( const std::string &json, Error &e )
    {
        auto tokenizer = JsonTokenizer( json );
        State state = State::Value;
        std::stack<Levels> levels;
        std::string token;
        while(true)
        {
            token = tokenizer.get_token( e );
            if ( token.empty() )
            {
                if ( !e.empty() )
                {
                    return false;
                }
                if ( !levels.empty() )
                {
                    auto pos = tokenizer.get_last_token_position();
                    e = Error( Json::Result::UnexpectedEnding, "Unexpected ending (%d:%d)", pos.first, pos.second );
                    return false;
                }
                break;
            }
            switch( state )
            {
            case State::Value:
                if ( is_lexeme( token ) || is_number( token ) || is_string( token ) )
                {
                    if ( levels.empty() )
                    {
                        state = State::End;
                    }
                    else
                    {
                        state = State::ValueSeparator;
                    }
                    continue;
                }
                else if ( token == "{" )
                {
                    state = State::Key;
                    levels.push( Levels::Object );
                    continue;
                }
                else if ( token == "[" )
                {
                    levels.push( Levels::Array );
                    continue;
                }
                else if ( token == "]" && !levels.empty() && levels.top() == Levels::Array )
                {
                    levels.pop();
                    continue;
                }
                break;
            case State::Key:
                if ( is_string( token ) )
                {
                    state = State::KeyValueSeparator;
                    continue;
                }
                else if ( token == "}" && !levels.empty() && levels.top() == Levels::Object )
                {
                    levels.pop();
                    state = State::ValueSeparator;
                    continue;
                }
                break;
            case State::KeyValueSeparator:
                if ( token == ":" )
                {
                    state = State::Value;
                    continue;
                }
                break;
            case State::ValueSeparator:
                if ( !levels.empty() )
                {
                    if ( token == "," )
                    {
                        state = levels.top() == Levels::Array ? State::Value : State::Key;
                        continue;
                    }
                    else if ( ( token == "}" && levels.top() == Levels::Object ) ||
                              ( token == "]" && levels.top() == Levels::Array ) )
                    {
                        levels.pop();
                        continue;
                    }
                }
                break;
            case State::End:
                if ( !token.empty() )
                {
                    auto pos = tokenizer.get_last_token_position();
                    e = Error( Json::Result::UnexpectedEnding, "Unexpected ending (%d:%d)", pos.first, pos.second );
                    return false;
                }
                continue;
            }
            auto pos = tokenizer.get_last_token_position();
            e = Error( Json::Result::UnexpectedToken, "Unexpected token (%d:%d)", pos.first, pos.second );
            return false;
        }
        return true;
    }
};


bool Json::validate( const std::string &json, Error &e )
{
    return JsonImpl::validate( json, e );
}

} // namespace Serialization
} // namespace UtilityLib
