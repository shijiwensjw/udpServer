cmake_minimum_required(VERSION 2.8.3)
project(udpServer)

SET(CMAKE_CXX_FLAGS "-std=c++11 -g -O3 ${CMAKE_CXX_FLAGS}")

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  message_generation
  tf
  pluginlib
  actionlib
  actionlib_msgs
  geometry_msgs
  std_msgs
  #udpServer
  
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES RobotNavigator MapInflationTool
  CATKIN_DEPENDS
    roscpp
    rospy
    tf
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(include ${catkin_INCLUDE_DIRS})

add_library(input_socket src/input.cc)

## Declare a cpp executable
add_executable(udpServer src/udpServer_node.cpp)
target_link_libraries(udpServer ${catkin_LIBRARIES} input_socket)





