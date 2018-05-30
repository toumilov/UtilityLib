cmake_minimum_required(VERSION 2.8)

include(CTest)

option(UTILITY_LIB_TESTS "Build unittests" ON)

if(${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4")
    set(CMAKE_CXX_COMPILER_ARG1 "-std=c++11" CACHE STRING "C++ version for eclipse" FORCE)
    set(CMAKE_ECLIPSE_VERSION "4.5" CACHE STRING "Eclipse version" FORCE)
endif()

if(DEFINED DEBUG)
	# Turn on debug symbols
	set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(UtilityLib)
if(UTILITY_LIB_TESTS)
	#add_definitions(-DCPPUTEST_MEM_LEAK_DETECTION_DISABLED)
	add_subdirectory(cpputest)
endif()
