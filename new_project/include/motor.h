#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>


#include <Arduino.h>
typedef struct 
{
    int16_t angle;
    int16_t speed;
    int16_t current;
    uint16_t temperature;
    /* data */
}motor_message;

extern void receive_information();
extern void transmit_information(int16_t ID, int16_t current);
extern float pid_control(float set_angle);
extern float motor_ctl_vel (float cmdPos);
#endif


