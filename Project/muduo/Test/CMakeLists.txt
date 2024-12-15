cmake_minimum_required(VERSION 3.0.0)
project(mudou)

set(CMAKE_BUILD_TYPE Debug)

add_executable(server TimerWheelServer.cc )
add_executable(client TimerWheelClient.cc )