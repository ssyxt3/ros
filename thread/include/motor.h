#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>



struct motor_message
{
    int16_t angle;
    int16_t speed;
    int16_t current;
    uint16_t temperature;
};

extern void pid_control();
extern void receive_information();
void transmit_information(int16_t cmdID);
extern float motor_ctl_vel (float cmdPos);
#endif


