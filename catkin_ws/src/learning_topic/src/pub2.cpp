#include <ros/ros.h>
#include "learning_topic/Person1.h"
#include "class.h"

int main (int argc, char** argv)
{
    ros::init (argc, argv, "node1");
    ros::NodeHandle n;
    ros::Publisher person2_info = n.advertise<learning_topic::Person> ("/person1_info",10);
    ros::Rate loop_rate (1);
    int count = 0;
    while (ros::ok ())
    {
        learning_topic::Person person2;
        person2.name = "A";
        person2.age = 100;
        person2.sex = 0;
        person2_info.publish (person2);
        ROS_INFO ("%s\t  %d\t  %d\n", person2.name.c_str (), person2.age, person2.sex);

        ros::spinOnce ();
        loop_rate.sleep ();
        count++;
    }
    return 0;
}