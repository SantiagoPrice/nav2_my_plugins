#include <string>
#include "is_goal_updated/is_goal_updated.hpp"

namespace is_goal_updated
{
    IsGoalUpdated::IsGoalUpdated(
        const std::string & condition_name,
        const BT::NodeConfiguration & conf)
        : BT::ConditionNode(condition_name , conf),
        goal_topic_("/best_point_marker"),
        yaw_corr_topic_("/yaw")
        {
            // Reads Entries
            getInput("goal_topic",goal_topic_);
            getInput("yaw_corr_topic",yaw_corr_topic_);
            // Setup callback Groups
            node_ = config().blackboard->get<rclcpp::Node::SharedPtr>("node");
            callback_group_ = node_->create_callback_group(
                rclcpp::CallbackGroupType::MutuallyExclusive,
                false);
            callback_group_executor_.add_callback_group(callback_group_, node_->get_node_base_interface());
            rclcpp::SubscriptionOptions sub_option;
            sub_option.callback_group = callback_group_;

            // Subscriber
            goal_sub_ = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
                goal_topic_,
                rclcpp::SystemDefaultsQoS(),
                std::bind(&IsGoalUpdated::GoalCallback, this, std::placeholders::_1),
                sub_option);
            
            if (yaw_corr_topic_ != "")
            {
                yawr_sub_ = node_->create_subscription<std_msgs::msg::Float32>(
                yaw_corr_topic_,
                rclcpp::SystemDefaultsQoS(),
                std::bind(&IsGoalUpdated::YawRCallback, this, std::placeholders::_1),
                sub_option);
            }

            // Timer whose callback executor matches the pcloud subscriber

            timer_ = node_->create_wall_timer(
                std::chrono::milliseconds(250),
                std::bind(&IsGoalUpdated::TimerCallback, this),
                callback_group_);

            
        }

    BT::NodeStatus IsGoalUpdated::tick()
    {
    callback_group_executor_.spin_some();
    if (new_goal_received_) {
        setOutput("target_goal", target_goal_);
        if (yaw_corr_topic_ == "")
        {
            return BT::NodeStatus::SUCCESS;
        } 
        else
        {
            if (new_yaw_received_ == true)
            {
                setOutput("yaw_correction", yawr_);
                return BT::NodeStatus::SUCCESS;
            }
        }
         
    }
    return BT::NodeStatus::FAILURE;
    }

    void IsGoalUpdated::GoalCallback(geometry_msgs::msg::PoseStamped::SharedPtr msg)
    {
        target_goal_ = *msg;
        new_goal_received_ = true;
    }
    void IsGoalUpdated::YawRCallback(std_msgs::msg::Float32::SharedPtr msg)
    {
        yawr_= msg->data;
        new_yaw_received_ = true;
    }
    void IsGoalUpdated::TimerCallback()
    {
        new_goal_received_ = false;
        new_yaw_received_  = false;
    }
}  // namespace is_goal_updated

#include "behaviortree_cpp_v3/bt_factory.h"
BT_REGISTER_NODES(factory)
{
    BT::NodeBuilder builder =
    [](const std::string & name, const BT::NodeConfiguration & config)
    {
      return std::make_unique<is_goal_updated::IsGoalUpdated>(name, config);
    };

  factory.registerBuilder<is_goal_updated::IsGoalUpdated>("IsGoalUpdated", builder);
}