#!/bin/bash

source /opt/ros/melodic/setup.sh
export HOSTNAME=$HOSTNAME
export ROS_HOSTNAME=$HOSTNAME.local

source catkin_ws/devel/setup.bash