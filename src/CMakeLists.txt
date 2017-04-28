cmake_minimum_required(VERSION 3.7)
project(src)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        net/net_base.cc
        net/net_base.h
        net/net_buffer_reader.h
        net/pack_util.h
        net/pack_base.h
        sample/main.cc)

add_executable(src ${SOURCE_FILES})