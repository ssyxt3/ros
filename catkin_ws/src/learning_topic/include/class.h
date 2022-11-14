#include <ros/ros.h>
#include "learning_topic/Person.h"
class sub_and_pub {
public:
    sub_and_pub ()
    {
        count = 0;
        pub1 = n.advertise<learning_topic::Person> ("/Person1_info", 10);
        pub2 = n.advertise<learning_topic::Person> ("/Person2_info", 10);
        sub1 = n.subscribe ("/Person2_info", 10, callback1,this);
        sub2 = n.subscribe ("/Person1_info", 10, callback2, this);
    }

    void callback1 (const learning_topic::Person::ConstPtr& msg1)
    {
        ROS_INFO ("Person1: name %s, age %d, sex %d", msg1->name, msg1->age, msg1->sex);
        output.age=msg1->age;
        output.name=msg1->name;
        output.sex=msg1->sex;
        pub1.publish (output);
    }

    void callback2 (const learning_topic::Person::ConstPtr& msg2)
    {
        ROS_INFO ("Person2: name %s, age %d, sex %d", msg2->name, msg2->age, msg2->sex);
        output.age=msg2->age;
        output.name=msg2->name;
        output.sex=msg2->sex;
        pub1.publish (output);
    }
    
private:
    ros::NodeHandle n;
    ros::Publisher pub1;
    ros::Publisher pub2;
    ros::Subscriber sub1;
    ros::Subscriber sub2;
    learning_topic::Person output;
    int count;
};