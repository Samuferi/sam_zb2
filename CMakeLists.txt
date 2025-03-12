cmake_minimum_required(VERSION 3.8)
project(sam_zb2)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
#find_package(geometry_msgs REQUIRED)

set(ament_dependencies
  rclcpp
  std_msgs
  geometry_msgs
)

add_executable(sensor_pub src/input_gen.cpp)
ament_target_dependencies(sensor_pub ${ament_dependencies} )
add_executable(processor_sub src/output_gen.cpp)
ament_target_dependencies(processor_sub ${ament_dependencies} )

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS
sensor_pub
processor_sub
DESTINATION lib/${PROJECT_NAME})

ament_package()