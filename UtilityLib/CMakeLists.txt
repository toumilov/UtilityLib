cmake_minimum_required(VERSION 2.8)

# Set compiler flags
SET( CMAKE_CXX_FLAGS  "-Wall -Werror -std=c++11" )
# Set linker flags
#SET( CMAKE_EXE_LINKER_FLAGS  "" )

set(sources
    Inc/Containers/Variant.hpp
    Inc/Containers/Value.hpp
    Src/Containers/Value.cpp
    Inc/Serialization/Base64.hpp
    Src/Serialization/Base64.cpp
    Inc/Error.hpp
    Src/Error.cpp
)

set(target_name UtilityLib)
add_library(${target_name} ${sources})
target_include_directories(${target_name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Inc)

if(UTILITY_LIB_TESTS)
    add_subdirectory(Test)
endif()
