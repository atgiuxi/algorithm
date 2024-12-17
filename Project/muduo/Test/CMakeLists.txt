cmake_minimum_required(VERSION 3.0.0)
project(mudou)

set(CMAKE_BUILD_TYPE Debug)

add_executable(server 4.TimerWheelServer.cc )
add_executable(client 4.TimerWheelClient.cc )
# add_executable(server 9.TcpServer.cc )
# add_executable(client 9.TcpClient.cc )