cmake_minimum_required(VERSION 3.8)
project(downward_ros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclpy REQUIRED)

ament_python_install_package(${PROJECT_NAME})

# Install Python executables
install(PROGRAMS
  downward_ros/fast-downward.py
  DESTINATION lib/${PROJECT_NAME}
)

set(py_cmd "python build.py release")
execute_process(
  COMMAND python build.py release
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
install(DIRECTORY
  builds
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
