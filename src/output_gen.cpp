#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/string.hpp"


class Processor_Sub: public rclcpp::Node
{
public:
    Processor_Sub(): Node("processor_sub")
    {
        subscription_ = this->create_subscription<std_msgs::msg::Float32MultiArray>("/sensor_data", 10, std::bind(&Processor_Sub::process_data, this, std::placeholders::_1));
        publisher_ = this->create_publisher<std_msgs::msg::Float32>("/processed_data", 10);
        publisher_2 = this->create_publisher<std_msgs::msg::String>("/direction_data", 10);
    }
private:
    void process_data(const std_msgs::msg::Float32MultiArray::SharedPtr msg)
    {
        std_msgs::msg::Float32 output_msg;
        float distance = msg->data[0];
        float distance_left = msg->data[1];
        float distance_right = msg->data[2];
        float deceleration_factor = (distance < 1.0) ? 1.0 : (5.0-distance)/4.0;
        output_msg.data = deceleration_factor;
        float max_dist=std::max(distance, std::max(distance_left,distance_right));
        std_msgs::msg::String side_choose;
        if(max_dist==distance) side_choose.data = "stay current";
        else if(max_dist==distance_left) side_choose.data = "turn left";
        else side_choose.data = "turn right";
        RCLCPP_INFO(this->get_logger(), "Deceleration factor: %f, Side to choose: %s", output_msg.data, side_choose.data.c_str());
        
        publisher_->publish(output_msg);
        publisher_2->publish(side_choose);
    }

    rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_2;

};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Processor_Sub>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}