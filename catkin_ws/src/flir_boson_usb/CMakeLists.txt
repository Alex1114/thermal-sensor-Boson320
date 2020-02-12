cmake_minimum_required(VERSION 2.8.3)
project(flir_boson_usb)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED
  COMPONENTS
  roscpp
  nodelet
  cv_bridge
  image_transport
  sensor_msgs
  camera_info_manager
  roslint
  roslib
)

find_package(OpenCV REQUIRED)
find_package(tf REQUIRED)
find_package(Eigen3 REQUIRED)

catkin_package(
  CATKIN_DEPENDS
  camera_info_manager
  image_transport
  cv_bridge
  nodelet
  sensor_msgs
)

include_directories(
  include
  ${OpenCV_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
  ${tf_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

message(STATUS ${tf_INCLUDE_DIRS})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
  ADD_DEFINITIONS(-DDEBUG)
ENDIF()


add_executable(${PROJECT_NAME}_node
  src/flir_boson_usb_node.cpp
)

add_dependencies(${PROJECT_NAME}_node
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(${PROJECT_NAME}_node
  ${OpenCV_LIBRARIES}
  ${catkin_LIBRARIES}
)

add_library(BosonCamera
  src/nodelets/BosonCamera.cpp
)

add_dependencies(BosonCamera
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

target_link_libraries(BosonCamera
  ${OpenCV_LIBRARIES}
  ${catkin_LIBRARIES}
)

set(ROSLINT_CPP_OPTS "--filter=-runtime/threadsafe_fn,-build/namespaces,-build/include_what_you_use")
roslint_cpp()

install(TARGETS ${PROJECT_NAME}_node BosonCamera
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY
  launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(FILES nodelet_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

add_library(align_thermal_to_rgb src/align_thermal_to_rgb.cpp)
target_link_libraries(align_thermal_to_rgb ${OpenCV_LIBRARIES} ${tf_LIBRARIES})

add_executable(ros_align_thermal src/ros_align_thermal.cpp)  
target_link_libraries(ros_align_thermal ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${libcbdetection_lib} ${tf_LIBRARIES} align_thermal_to_rgb) 


