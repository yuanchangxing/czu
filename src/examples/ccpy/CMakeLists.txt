#@songtzu
#songtzu@126.com
#4.29.2017

cmake_minimum_required(VERSION 3.7)
project(ccpy)

#set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/src/sample)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
#set(PROJECT_BINARY_DIR ${PROJECT_SOURCE_DIR}/src/sample)


set(CMAKE_CXX_STANDARD 14)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

set( CMAKE_BUILD_TYPE Debug )
#find_package(PythonLibs 2.7.12 EXACT REQUIRED)
find_package(PythonLibs  REQUIRED)




include_directories(${PYTHON_INCLUDE_DIRS})


INCLUDE_DIRECTORIES("./")
FILE(GLOB sample "./*.cc")

# proto define
FILE(GLOB proto_src "../../proto/*.proto")



#tools dir
INCLUDE_DIRECTORIES("../../tools/")
FILE(GLOB tools "../../tools/*.c*")

#threadpool dir
INCLUDE_DIRECTORIES("../../tools/threadpool/")
FILE(GLOB threadpool "../../tools/threadpool/*.cc")

#proto
INCLUDE_DIRECTORIES("../../../proto")
INCLUDE_DIRECTORIES("../../proto")
FILE(GLOB proto "../../proto/*.cc")


#net
INCLUDE_DIRECTORIES("../../net/")
FILE(GLOB net "../../net/*.cc")



LINK_DIRECTORIES("/usr/local/lib/")

AUX_SOURCE_DIRECTORY(. DIR_SRCS)

ADD_EXECUTABLE(ccpy ${DIR_SRCS} ${proto_src} ${sample} ${tools} ${threadpool} ${proto} ${net} )

TARGET_LINK_LIBRARIES(ccpy protobuf -lpthread ${PYTHON_LIBRARIES} )

#add_executable(czu ${SOURCE_FILES} src/tools/clog.h)