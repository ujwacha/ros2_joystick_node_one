// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <functional>
#include <memory>
#include <rclcpp/logging.hpp>
#include <rclcpp/qos.hpp>

// #include <unistd.h>
// #include <fcntl.h>
// #include <termios.h>
// #include <string.h>


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"


using std::placeholders::_1;

class MinimalSubscriber : public rclcpp::Node
{
public:



  MinimalSubscriber()
  : Node("minimal_subscriber")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&MinimalSubscriber::topic_callback, this, _1));


    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel_base", 10);

  }




private:
 
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;



  float valx;
  float valy;
  float pyaw;  
  float nyaw;
  float totalyaw;

  
  


  void topic_callback(const sensor_msgs::msg::Joy & msg) 
  {
    //        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());


    // for (int i = 0; i < 5; i++)
    //   {
    // 	RCLCPP_INFO(this->get_logger(), "%i",  msg.buttons[i]);
    //   }
 
    // for (int i = 0; i < 5; i++)
    //   {
    // 	RCLCPP_INFO(this->get_logger(), "%f",  msg.axes[i]);
    //   }


    valx = -1.0 * (float) msg.axes[0];
    valy = (float)msg.axes[1];
    pyaw = (float)msg.axes[4];
    nyaw = (float)msg.axes[5];

    my_set_total_yaw();

    
    RCLCPP_INFO(this->get_logger(), "valx: %f", valx);
    RCLCPP_INFO(this->get_logger(), "valy: %f", valy);
    RCLCPP_INFO(this->get_logger(), "tyaw: %f", totalyaw);

    
    publish_message(valx, valy, totalyaw);
    
  }


  void my_set_total_yaw() {
    float t1 = 0.75 * (1.0 - pyaw);
    float t2 = 0.75 * (1.0 - nyaw);

    totalyaw = t1 - t2;
    
  }

  void publish_message(float vx, float vy, float ty) {

    auto message = geometry_msgs::msg::Twist();

    message.linear.x = vx;
    message.linear.y = vy;


    message.angular.z = ty;

    publisher_->publish(message);

    
  }
 

  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
};


// class SerialPublisher
// 

// public:
//   SerialPublisher(char* port) {

//     fd = open(port, O_RDWR | O_NDELAY | O_NOCTTY);


//     is_readable = (fd > 0);
    
//   }

  

// private:
//   bool is_readable ;
//   int fd;
//   char buffer[255];
//   struct termios operation;
  
// };


int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalSubscriber>());
  rclcpp::shutdown();
  return 0;
}
