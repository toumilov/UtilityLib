cmake_minimum_required(VERSION 2.8)

# Set compiler flags
SET( CMAKE_CXX_FLAGS  "-std=c++11" )
# Set linker flags
#SET( CMAKE_EXE_LINKER_FLAGS  "" )

set(sources Main.cpp
	Serialization/Base64Test.cpp
)

set(UNIT_TEST unittest)
add_executable(${UNIT_TEST} ${sources})
target_link_libraries(${UNIT_TEST} LINK_PUBLIC UtilityLib CppUTest pthread)
add_dependencies(${UNIT_TEST} UtilityLib)

enable_testing()
add_test(NAME Base64Test COMMAND unittest -g Base64Test)

add_custom_command(
	TARGET ${UNIT_TEST}
	COMMENT "Runing unit tests"
	POST_BUILD 
	COMMAND ctest -C $<CONFIGURATION> --output-on-failure
)
