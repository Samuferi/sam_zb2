#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include <random>

class SensorPub: public rclcpp::Node
{
public:
    SensorPub(): Node("sensor_pub"), gen(rd()), dist_curr(0.2, 5.0), dist_to_left(0.2, 5.0), dist_to_right(0.2, 5.0)
    {
        publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/sensor_data", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1000), std::bind(&SensorPub::publish_data, this));
    }

private:
    void publish_data()
    {
        std_msgs::msg::Float32MultiArray msg;
        msg.data = {dist_curr(gen), dist_to_left(gen), dist_to_right(gen)};
        RCLCPP_INFO(this->get_logger(), "Publishing: %f, %f, %f", msg.data[0], msg.data[1], msg.data[2]);
        publisher_->publish(msg);
    }

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dist_curr;
    std::uniform_real_distribution<float> dist_to_left;
    std::uniform_real_distribution<float> dist_to_right;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SensorPub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}