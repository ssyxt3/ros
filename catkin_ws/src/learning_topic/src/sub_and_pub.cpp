#include <ros/ros.h>
#include <learning_topic/Person1.h>
#include <boost/thread.hpp>
#include "class.h"

int main (int argc, char** argv)
{
    ros::init (argc, argv, "Subscriber_and_publisher");
    sub_and_pub test;
    ros::MultiThreadedSpinner s (2);
    ros::spin (s);
    return 0;
}

// void sub_and_pub::callback2 (const learning_topic::Person::ConstPtr& msg2)
// {
//     ROS_INFO("get info: %s",)
// }
