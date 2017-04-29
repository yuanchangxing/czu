#@songtzu
#songtzu@126.com
#4.29.2017

cmake_minimum_required(VERSION 3.7)
project(czu)

set(CMAKE_CXX_STANDARD 11)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#set( CMAKE_BUILD_TYPE Debug )


INCLUDE_DIRECTORIES("./src/sample/")
FILE(GLOB sample "./src/sample/*.cc")

# proto define
FILE(GLOB proto_src "./src/proto/*.proto")



#tools dir
INCLUDE_DIRECTORIES("./src/tools/")
FILE(GLOB tools "./src/tools/*.c*")

#threadpool dir
INCLUDE_DIRECTORIES("./src/tools/threadpool/")
FILE(GLOB threadpool "./src/tools/threadpool/*.cc")

#proto
INCLUDE_DIRECTORIES("../proto")
FILE(GLOB proto "./src/proto/*.cc")


#net
INCLUDE_DIRECTORIES("./src/net/")
FILE(GLOB net "./src/net/*.cc")



LINK_DIRECTORIES("/usr/local/lib/")

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(czu ${DIR_SRCS} ${proto_src} ${sample} ${tools} ${threadpool} ${proto} ${net} )

TARGET_LINK_LIBRARIES(czu protobuf -lpthread )

#add_executable(czu ${SOURCE_FILES} src/tools/clog.h)