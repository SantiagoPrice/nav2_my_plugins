#ifndef IS_TARGET_VISIBLE__IS_VISIBLE_TARGET_HPP_
#define IS_TARGET_VISIBLE__IS_VISIBLE_TARGET_HPP_

#include<string>

#include "rclcpp/rclcpp.hpp"
#include <sensor_msgs/msg/point_cloud2.hpp>
#include "behaviortree_cpp_v3/condition_node.h"


namespace is_target_visible
{
/**
 * @brief A BT::ConditionNode that listens to the ultralitics node and
 * returns SUCCESS when receiving a planecloud message in the last second and FAILURE otherwise
 */
class IsTargetVisible : public BT::ConditionNode
{
public:
  IsTargetVisible(
    const std::string & condition_name,
    const BT::NodeConfiguration & conf);

  IsTargetVisible() = delete;

  /**
  * @brief The main override required by a BT action
  * @return BT::NodeStatus Status of tick execution
  */
  BT::NodeStatus tick() override;
  
  static BT::PortsList providedPorts()
  {
    return{
      BT::InputPort<std::string>("PCloud_Topic", "plane_cloud","Topic of the published plane cloud"),
      BT::OutputPort<bool>("is_visible","Is true if the target is visible"),
    };
  }

private:
  void oVisibleCallback(sensor_msgs::msg::PointCloud2::SharedPtr msg);
  void TimerCallback();
  rclcpp::Node::SharedPtr node_;
  rclcpp::CallbackGroup::SharedPtr callback_group_;
  rclcpp::executors::SingleThreadedExecutor callback_group_executor_;
  rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcloud_sub_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string pcloud_topic_;
  bool is_visible_;
};

}  // namespace is_target_visible

#endif  // IS_TARGET_VISIBLE__IS_VISIBLE_TARGET_HPP_
