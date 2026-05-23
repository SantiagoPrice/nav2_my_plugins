cmake_minimum_required(VERSION 3.8)
project(is_target_visible)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_ros REQUIRED)
find_package(rclcpp REQUIRED)
find_package(nav2_behavior_tree REQUIRED)
find_package(behaviortree_cpp_v3 REQUIRED)
find_package(pluginlib REQUIRED)
find_package(ultralytics_ros REQUIRED)
find_package(sensor_msgs REQUIRED)

add_library(is_target_visible_bt_node SHARED src/is_target_visible.cpp)
target_compile_features(is_target_visible_bt_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_include_directories(is_target_visible_bt_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(
  is_target_visible_bt_node
  "rclcpp"
  "nav2_behavior_tree"
  "behaviortree_cpp_v3"
  "pluginlib"
  "ultralytics_ros"
)

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(is_target_visible_bt_node PRIVATE BT_PLUGIN_EXPORT)

install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS is_target_visible_bt_node
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
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

pluginlib_export_plugin_description_file(nav2_behavior_tree is_target_visible.xml)

ament_export_dependencies(
  rclcpp
  nav2_behavior_tree
  behaviortree_cpp_v3
  pluginlib
  ultralytics_ros
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
