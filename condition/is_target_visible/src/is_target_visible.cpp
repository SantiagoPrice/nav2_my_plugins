#include <string>
#include "is_target_visible/is_target_visible.hpp"

namespace is_target_visible
{
    IsTargetVisible::IsTargetVisible(
        const std::string & condition_name,
        const BT::NodeConfiguration & conf)
        : BT::ConditionNode(condition_name , conf),
        pcloud_topic_("/plane_cloud"),
        is_visible_(false)
        {
            // Reads Entries
            getInput("PCloud_Topic",pcloud_topic_);
            getInput("is_visible",is_visible_);

            // Setup callback Groups
            node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");
            callback_group_ = node_->create_callback_group(
                rclcpp::CallbackGroupType::MutuallyExclusive,
                false);
            callback_group_executor_.add_callback_group(callback_group_, node_->get_node_base_interface());
            rclcpp::SubscriptionOptions sub_option;
            sub_option.callback_group = callback_group_;

            // Subscriber
            pcloud_sub_ = node_->create_subscription<sensor_msgs::msg::PointCloud2>(
                pcloud_topic_,
                rclcpp::SystemDefaultsQoS(),
                std::bind(&IsTargetVisible::oVisibleCallback, this, std::placeholders::_1),
                sub_option);

            // Timer whose callback executor matches the pcloud subscriber

            timer_ = node_->create_wall_timer(
                std::chrono::milliseconds(250),
                std::bind(&IsTargetVisible::TimerCallback, this),
                callback_group_);

            
        }

    BT::NodeStatus IsTargetVisible::tick()
    {
    callback_group_executor_.spin_some();
    setOutput("is_visible", is_visible_);
    if (is_visible_) {
        return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::FAILURE;
    }

    void IsTargetVisible::oVisibleCallback(sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {
        is_visible_ = true;
    }
    void IsTargetVisible::TimerCallback()
    {
        is_visible_ = false;
    }
}  // namespace is_target_visible

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
    BT::NodeBuilder builder =
    [](const std::string & name, const BT::NodeConfiguration & config)
    {
      return std::make_unique<is_target_visible::IsTargetVisible>(name, config);
    };

  factory.registerBuilder<is_target_visible::IsTargetVisible>("IsTargetVisible", builder);

//   factory.registerNodeType<is_target_visible::IsTargetVisible>("IsTargetVisible");
}