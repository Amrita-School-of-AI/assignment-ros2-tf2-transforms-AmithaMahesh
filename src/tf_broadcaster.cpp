#include <chrono>
#include <functional>
#include <memory>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class TFBroadcaster : public rclcpp::Node
{
public:
    TFBroadcaster()
        : Node("tf_broadcaster"), time_(0.0)
    {
        // Create broadcaster
        tf_broadcaster_ =
            std::make_unique<tf2_ros::TransformBroadcaster>(*this);

        // Create timer (100 ms)
        timer_ = this->create_wall_timer(
            100ms, std::bind(&TFBroadcaster::timer_callback, this));
    }

private:
    void timer_callback()
    {
        geometry_msgs::msg::TransformStamped t;

        // Header
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "world";
        t.child_frame_id = "robot";

        // Circular motion
        double x = 2.0 * std::cos(time_);
        double y = 2.0 * std::sin(time_);

        t.transform.translation.x = x;
        t.transform.translation.y = y;
        t.transform.translation.z = 0.0;

        // Identity quaternion
        t.transform.rotation.x = 0.0;
        t.transform.rotation.y = 0.0;
        t.transform.rotation.z = 0.0;
        t.transform.rotation.w = 1.0;

        // Broadcast
        tf_broadcaster_->sendTransform(t);

        // advance time
        time_ += 0.1;
    }

    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr timer_;
    double time_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TFBroadcaster>());
    rclcpp::shutdown();
    return 0;
}