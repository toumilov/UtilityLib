cmake_minimum_required(VERSION 2.8)

# Set compiler flags
SET( CMAKE_CXX_FLAGS  "-std=c++11" )
# Set linker flags
#SET( CMAKE_EXE_LINKER_FLAGS  "" )

set(sources Main.cpp
    Containers/VariantTest.cpp
    Containers/ValueTest.cpp
    Serialization/Base64Test.cpp
    Serialization/JsonTest.cpp
    Serialization/Utf8Test.cpp
    ErrorTest.cpp
)

set(UNIT_TEST unittest)
add_executable(${UNIT_TEST} ${sources})
target_link_libraries(${UNIT_TEST} LINK_PUBLIC UtilityLib CppUTest pthread)
add_dependencies(${UNIT_TEST} UtilityLib)

enable_testing()
add_test(NAME Base64Group COMMAND ${UNIT_TEST} -g Base64Group)
add_test(NAME JsonValidationGroup COMMAND ${UNIT_TEST} -g JsonValidationGroup)
add_test(NAME Utf8ValidationGroup COMMAND ${UNIT_TEST} -g Utf8ValidationGroup)
add_test(NAME ErrorGroup COMMAND ${UNIT_TEST} -g ErrorGroup)
add_test(NAME VariantGroup COMMAND ${UNIT_TEST} -g VariantGroup)
add_test(NAME ValueGroup COMMAND ${UNIT_TEST} -g ValueGroup)

add_custom_command(
    TARGET ${UNIT_TEST}
    COMMENT "Running unit tests"
    POST_BUILD
    COMMAND ctest -C $<CONFIGURATION> --output-on-failure
)
