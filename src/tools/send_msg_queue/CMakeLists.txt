project(send_msg_queue)

set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 2.8)
message("send_msg_queue")



FILE(GLOB src "*.cc")

include_directories("./")

#add_subdirectory(lib)

add_library(send_msg_queue SHARED ${src})

message("send_msg_queue is finished.")