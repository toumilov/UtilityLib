#pragma once

#include <cmath>
#include <cstdint>
#include <vector>
#include <string>
#include <map>
#include "Containers/Variant.hpp"


namespace UtilityLib
{
namespace Containers
{

class Value
{
public:
    enum class Type
    {
        None = 0,
        Int32,
        Int64,
        Uint32,
        Uint64,
        Float,
        Double,
        Bool,
        String,
        Array,
        Object
    };
    typedef void* None;
    typedef int32_t Int32;
    typedef int64_t Int64;
    typedef uint32_t Uint32;
    typedef uint64_t Uint64;
    typedef float Float;
    typedef double Double;
    typedef bool Bool;
    typedef std::string String;
    typedef std::vector<Value> Array;
    typedef std::map<std::string, Value> Object;

    Value();
    Value( const Type t );
    Value( const char *s );

    template <typename T>
    Value( const T &value )
    {
        data_ = value;
        type_ = type();
    }

    Value& operator=( const char *value );

    template<typename T,
             typename std::enable_if <std::is_same<T, None>::value   ||
                                      std::is_same<T, Int32>::value  ||
                                      std::is_same<T, Int64>::value  ||
                                      std::is_same<T, Uint32>::value ||
                                      std::is_same<T, Uint64>::value ||
                                      std::is_same<T, Float>::value  ||
                                      std::is_same<T, Double>::value ||
                                      std::is_same<T, Bool>::value   ||
                                      std::is_same<T, String>::value ||
                                      std::is_same<T, Array>::value  ||
                                      std::is_same<T, Object>::value>::type* = nullptr>
    Value& operator=( const T &value )
    {
        data_ = value;
        type_ = type();
        return *this;
    }

    template<typename T,
             typename std::enable_if<std::is_same<T, None>::value   ||
                                     std::is_same<T, Int32>::value  ||
                                     std::is_same<T, Int64>::value  ||
                                     std::is_same<T, Uint32>::value ||
                                     std::is_same<T, Uint64>::value ||
                                     std::is_same<T, Float>::value  ||
                                     std::is_same<T, Double>::value ||
                                     std::is_same<T, Bool>::value   ||
                                     std::is_same<T, String>::value>::type* = nullptr>
    bool operator==( const T &value ) const
    {
        auto p_data = data_.get<T>();
        return is<T>() && p_data && value == *p_data;
    }

    bool operator==( const char *value ) const;
    bool operator==( const Value &value ) const;

    template <typename T>
    bool operator!=( const T &value ) const
    {
        return !( *this == value );
    }

    Value& operator[]( unsigned index );
    Value& at( unsigned index );
    Value& insert( Value &&v );
    Value& erase( unsigned index );
    Value& operator[]( const std::string &key );
    Value& at( const std::string &key );
    Value& insert( const std::string &key, Value &&v );
    Value& erase( const std::string &key );

    template<typename T,
             typename std::enable_if <std::is_same<T, None>::value   ||
                                      std::is_same<T, Int32>::value  ||
                                      std::is_same<T, Int64>::value  ||
                                      std::is_same<T, Uint32>::value ||
                                      std::is_same<T, Uint64>::value ||
                                      std::is_same<T, Float>::value  ||
                                      std::is_same<T, Double>::value ||
                                      std::is_same<T, Bool>::value   ||
                                      std::is_same<T, String>::value ||
                                      std::is_same<T, Array>::value  ||
                                      std::is_same<T, Object>::value>::type* = nullptr>
    bool is() const
    {
        const T* ptr;
        return type( ptr ) == type_;
    }

    template <typename T>
    T& get()
    {
        static T t = default_value<T>();
        auto v = data_.get<T>();
        return v ? *v : t;
    }

    template <typename T>
    const T& get() const
    {
        auto v = data_.get<T>();
        return v ? *v : default_value<T>();
    }

    template <typename T>
    static const T& default_value()
    {
        const T* ptr = nullptr;
        return default_value( ptr );
    }

    template<typename T>
    Type type( const T *v ) const
    {
        static struct
        {
            Value::Type t = Value::Type::None;
            void operator()( const None* )   { t = Type::None;   }
            void operator()( const Int32* )  { t = Type::Int32;  }
            void operator()( const Int64* )  { t = Type::Int64;  }
            void operator()( const Uint32* ) { t = Type::Uint32; }
            void operator()( const Uint64* ) { t = Type::Uint64; }
            void operator()( const Float* )  { t = Type::Float;  }
            void operator()( const Double* ) { t = Type::Double; }
            void operator()( const Bool* )   { t = Type::Bool;   }
            void operator()( const String* ) { t = Type::String; }
            void operator()( const Array* )  { t = Type::Array;  }
            void operator()( const Object* ) { t = Type::Object; }
        } visitor;

        data_.accept( v, visitor );
        return visitor.t;
    }

    Type type() const;
    bool is( Type t ) const;
    Value as( Type t ) const;
    bool is_none() const;
    bool empty() const;
    void clear();
    unsigned size() const;
    bool is_convertable( const Type t ) const;

private:
    Type type_;
    Variant<
        None,
        Bool,
        Int32,
        Int64,
        Uint32,
        Uint64,
        Float,
        Double,
        String,
        Array,
        Object
    > data_;

    static const Int32 default_int32_;
    static const Int64 default_int64_;
    static const Uint32 default_uint32_;
    static const Uint64 default_uint64_;
    static const Float default_float_;
    static const Double default_double_;
    static const Bool default_bool_;
    static const String default_string_;
    static const Array default_array_;
    static const Object default_object_;

    static const Int32&  default_value( const Int32* );
    static const Int64&  default_value( const Int64* );
    static const Uint32& default_value( const Uint32* );
    static const Uint64& default_value( const Uint64* );
    static const Float&  default_value( const Float* );
    static const Double& default_value( const Double* );
    static const Bool&   default_value( const Bool* );
    static const String& default_value( const String* );
    static const Array&  default_value( const Array* );
    static const Object& default_value( const Object* );
};


template<typename T>
bool operator==( const T &lhs, const Value &rhs )
{
    return rhs.operator==( lhs );
}

} // namespace Containers
} // namespace UtilityLib
