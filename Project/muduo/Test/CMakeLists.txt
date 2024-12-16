cmake_minimum_required(VERSION 3.0.0)
project(mudou)

set(CMAKE_BUILD_TYPE Debug)

add_executable(server 8.LoopThreadPoolServer.cc )
add_executable(client 7.LoopThreadClient.cc )