#include <ros/ros.h>
#include "learning_topic/Person1.h"
#include "class.h"

int main (int argc, char** argv)
{
    ros::init (argc, argv, "node1");
    ros::NodeHandle n;
    ros::Publisher person1_info = n.advertise<learning_topic::Person> ("/person1_info",10);
    ros::Rate loop_rate (1);
    int count = 0;
    while (ros::ok ())
    {
        learning_topic::Person person1;
        person1.name = "A";
        person1.age = 100;
        person1.sex = 0;
        person1_info.publish (person1);
        ROS_INFO ("%s\t  %d\t  %d\n", person1.name.c_str (), person1.age, person1.sex);
        ros::spinOnce ();
        loop_rate.sleep ();
    }
    return 0;
}