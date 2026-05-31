cmake_minimum_required(VERSION 3.8)
project(spin_reactive_action CXX)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(nav2_behavior_tree REQUIRED)
find_package(nav2_util REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(pluginlib REQUIRED)
find_package(sensor_msgs REQUIRED)




add_library(spin_reactive_action_bt_node SHARED src/spin_reactive_action.cpp)
target_include_directories(spin_reactive_action_bt_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

ament_target_dependencies(
  spin_reactive_action_bt_node
  "rclcpp"
  "rclcpp_action"
  "nav2_behavior_tree"
  "nav2_util"
  "nav2_msgs"
  "behaviortree_cpp_v3"
  "pluginlib"
)
target_compile_definitions(spin_reactive_action_bt_node PRIVATE BT_PLUGIN_EXPORT)

install(
  DIRECTORY include/
  DESTINATION include
)

install(TARGETS spin_reactive_action_bt_node
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

pluginlib_export_plugin_description_file(nav2_behavior_tree spin_reactive_action.xml)

ament_export_dependencies(
  rclcpp
  rclcpp_action 
  nav2_behavior_tree
  nav2_util
  nav2_msgs
  behaviortree_cpp_v3
  pluginlib
  sensor_msgs
)

ament_export_include_directories(
  include
)
ament_export_libraries(
  is_target_visible_bt_node
)
ament_export_targets(
  export_${PROJECT_NAME}
)

ament_package()
