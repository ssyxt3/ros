#include <Arduino.h>
#include "pid.h"
#include "motor.h"
#include "driver/can.h"
#include "driver/gpio.h"

motor_message message;

void transmit_information(int16_t ID, int16_t current)
{
    can_message_t tx_message;
    tx_message.identifier = ID;
    tx_message.flags = CAN_MSG_FLAG_NONE;
    tx_message.data_length_code = 8;
    for(int i = 0; i<4; i++)
    {
        tx_message.data[i*2] = current >> 8;
        tx_message.data[i*2+1] = current;
    }
    
    can_transmit(&tx_message,1);
}



void receive_information()
{
    can_message_t rx_messsage;
    can_receive(&rx_messsage,100);
    switch(rx_messsage.identifier )
    {
        case 0x205:
        message.angle =  ((rx_messsage.data[0] << 8)| rx_messsage.data[1]);
        message.speed = (int16_t)((rx_messsage.data[2] << 8) | rx_messsage.data[3]);
        message.current = (int16_t)((rx_messsage.data[4] << 8) | rx_messsage.data[5]);
        message.temperature = (int16_t)rx_messsage.data[5];
        break;

        default:
        break;
    }
    Serial.printf("angle: %d\n",message.angle);
    Serial.printf("speed: %d\n",message.speed);
    Serial.printf("current: %d\n",message.current);

}

float pid_position[3] = {1.0f,0.0f,0.0f};
float pid_velcity[3] = {20.0f,5.0f,5.0f};

float pid_control(float set_angle)
{
    const float max_current = 30000;
    const float max_ki_current =8000;
    const float max_angle = 132;
    const float max_ki_angle = 10;
    pid_type_def pidPositionStruct;
    pid_type_def pidVelcityStruct;

    int16_t current;
    int16_t velcity;
    PID_init(&pidPositionStruct,PID_POSITION,pid_position,max_angle,max_ki_angle);
    PID_init (&pidVelcityStruct, PID_POSITION,pid_velcity,max_current,max_ki_current);
    velcity = PID_calc(&pidPositionStruct,message.angle,set_angle/ 360.0f * 8191.0f);
    current = PID_calc(&pidVelcityStruct, message.speed,velcity);
    return current;
}






