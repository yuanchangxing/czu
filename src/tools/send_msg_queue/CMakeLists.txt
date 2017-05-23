project(send_msg_queue)
cmake_minimum_required(VERSION 2.8)

set(src "*.cc")

include_directories("./")


add_library(send_msg_queue SHARED ${src})