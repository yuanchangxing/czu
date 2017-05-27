
project(send_msg_sample)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

INCLUDE_DIRECTORIES("./")
FILE(GLOB sample "./*.cc")

INCLUDE_DIRECTORIES("../")
FILE(GLOB lib "../*.cc")


ADD_EXECUTABLE(send_msg_sample ${sample} ${lib} )

TARGET_LINK_LIBRARIES(send_msg_sample -lpthread )