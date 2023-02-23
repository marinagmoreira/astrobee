# Copyright (c) 2017, United States Government, as represented by the
# Administrator of the National Aeronautics and Space Administration.
# 
# All rights reserved.
# 
# The Astrobee platform is licensed under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with the
# License. You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations
# under the License.

cmake_minimum_required(VERSION 3.5)
project(data_bagger)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC" )

# Find amend and libraries
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)

find_package(ff_common REQUIRED)
find_package(ff_util REQUIRED)
find_package(ff_msgs REQUIRED)
find_package(config_reader REQUIRED)

###########
## Build ##
###########

# Specify additional locations of header files
include_directories(
  include
)

# Declare C++ libraries
add_library(astrobee_recorder
  src/astrobee_recorder.cc
)
target_link_libraries(astrobee_recorder ${catkin_LIBRARIES})
ament_target_dependencies(astrobee_recorder rclcpp)

# Declare C++ libraries
add_library(data_bagger SHARED
  src/data_bagger.cc
)
target_compile_definitions(data_bagger
  PRIVATE "COMPOSITION_BUILDING_DLL")
target_link_libraries(data_bagger astrobee_recorder)
ament_target_dependencies(data_bagger rclcpp rclcpp_components ff_util)
rclcpp_components_register_nodes(data_bagger "data_bagger::DataBagger")

#############
## Install ##
#############

ament_export_include_directories(include)

# Mark libraries for installation
install(TARGETS astrobee_recorder
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Mark launch files for installation
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
  PATTERN ".svn" EXCLUDE)

ament_package()
