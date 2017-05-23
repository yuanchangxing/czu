project(send_msg_queue)
cmake_minimum_required(VERSION 2.8)

FILE(GLOB src "*.cc")

include_directories("./")

#ADD_LIBRARY(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)

add_library(send_msg_queue SHARED ${src})