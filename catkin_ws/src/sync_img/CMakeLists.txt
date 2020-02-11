cmake_minimum_required(VERSION 2.8.3)
project(sync_img)

find_package(catkin REQUIRED COMPONENTS message_filters roscpp sensor_msgs)

catkin_package(
  CATKIN_DEPENDS message_filters roscpp sensor_msgs
)

include_directories(${catkin_INCLUDE_DIRS})

add_executable(sync_img src/sync_img.cpp)
target_link_libraries(sync_img ${catkin_LIBRARIES})