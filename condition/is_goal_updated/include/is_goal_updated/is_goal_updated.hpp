#ifndef IS_GOAL_UPDATED__IS_GOAL_UPDATED_HPP_
#define IS_GOAL_UPDATED__IS_GOAL_UPDATED_HPP_

#include<string>

#include "rclcpp/rclcpp.hpp"
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <std_msgs/msg/float32.hpp>
#include "behaviortree_cpp_v3/condition_node.h"
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

namespace is_goal_updated
{
/**
 * @brief A BT::ConditionNode that listens to the ultralitics node and
 * returns SUCCESS when receiving a planecloud message in the last second and FAILURE otherwise
 */
class IsGoalUpdated : public BT::ConditionNode
{
public:
  IsGoalUpdated(
    const std::string & condition_name,
    const BT::NodeConfiguration & conf);

  IsGoalUpdated() = delete;

  /**
  * @brief The main override required by a BT action
  * @return BT::NodeStatus Status of tick execution
  */
  BT::NodeStatus tick() override;
  
  static BT::PortsList providedPorts()
  {
    return{
      BT::InputPort<std::string>("goal_topic", "best_point_marker","Topic of the published goal"),
      BT::InputPort<std::string>("yaw_corr_topic", "","(Optional) Yaw angle from the goal for the robot to face the target front edge"),
      BT::OutputPort<geometry_msgs::msg::PoseStamped>("target_goal","Pose of the goal"),
      BT::OutputPort<double>("yaw_correction","Correction angle")
    };
  }

private:
  void GoalCallback(geometry_msgs::msg::PoseStamped::SharedPtr msg);
  void YawRCallback(std_msgs::msg::Float32::SharedPtr msg);
  void TimerCallback();
  rclcpp::Node::SharedPtr node_;
  rclcpp::CallbackGroup::SharedPtr callback_group_;
  rclcpp::executors::SingleThreadedExecutor callback_group_executor_;
  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr goal_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr yawr_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string goal_topic_;
  std::string yaw_corr_topic_;
  geometry_msgs::msg::PoseStamped target_goal_;
  double yawr_; 
  bool new_goal_received_ = false;
  bool new_yaw_received_ = false;
};

}  // namespace is_goal_updated

#endif  // IS_GOAL_UPDATED__IS_GOAL_UPDATED_HPP_
