# Main project declaration
PROJECT(ProtoBot CXX C)

# Verbose or not verbose
IF($ENV{VERBOSE})
	MESSAGE(STATUS "Verbose mode on")
ELSE($ENV{VERBOSE})
	MESSAGE(STATUS "Verbose mode off (type 'VERBOSE=1 cmake .' to enable it)")
ENDIF($ENV{VERBOSE})

# Needed by CMake versione 2.6 or later (Run "cmake --help-policy CMP0003" for more information.)"
# specifically, we are using full-path when possible for linking libraries
# this should be mandatory for packages found using ROOT directive
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Optional flags and configurable stuff in ccmake
# Replacement for manual.cmake
SET(COMPILATION_FLAGS "-W -Wall -O2 -ggdb" CACHE STRING "CMake compilation flags")
SET(CMAKE_BUILD_TYPE "Debug")

SET(OOML_INCLUDE_DIR "/usr/include/ooml" CACHE STRING "OOML Include Dir")
SET(OOML_LINK_DIR "/usr/lib" CACHE STRING "OOML Libraries Directory")

INCLUDE_DIRECTORIES(${OOML_INCLUDE_DIR})
LINK_DIRECTORIES(${OOML_LINK_DIR})

ADD_EXECUTABLE(protobot protobot.cpp)
TARGET_LINK_LIBRARIES(protobot OOMLCore OOMLParts OOMLComponents)
