from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='sam_zb2',
            executable='sensor_pub',
        ),
        Node(
            package='sam_zb2',
            executable='processor_sub',
        )
    ])

