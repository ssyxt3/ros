#include <Arduino.h>
#include "pid.h"
#include "motor.h"
#include "driver/can.h"
#include "driver/gpio.h"

extern pid_type_def pidPositionStruct;
extern pid_type_def pidVelcityStruct;
extern float angle_ch0;
extern float angle_ch1;
can_message_t rx_messsage;
twai_status_info_t state;
motor_message message[7];
int16_t current[2];

void transmit_information(int16_t cmdID)
{
    can_message_t tx_message;
    tx_message.identifier = cmdID;
    tx_message.flags = CAN_MSG_FLAG_NONE;
    tx_message.data_length_code = 8;

    switch (tx_message.identifier)
    {
    case 0x1ff:
        for (int i = 0; i < 4; i++)
        {
            tx_message.data[i * 2] = current[i] >> 8;
            tx_message.data[i * 2 + 1] = current[i];
        }
        break;
    case 0x2ff:
        for (int i = 0; i < 4; i++)
        {
            tx_message.data[i * 2] = current[i + 4] >> 8;
            tx_message.data[i * 2 + 1] = current[i + 4];
        }
        break;
    }

    can_transmit(&tx_message, 1);
}

void receive_information()
{
    twai_get_status_info(&state);
    if (state.msgs_to_rx != 0)
    {
        can_receive(&rx_messsage, 1);

        if (rx_messsage.identifier > 0x204 && rx_messsage.identifier < 0x20C)
        {
            uint8_t receive_id = rx_messsage.identifier - 0x204;

            message[receive_id - 1].angle = (int16_t)((rx_messsage.data[0] << 8) | rx_messsage.data[1])* 360.0f / 8191.0f;
            message[receive_id - 1].speed = (int16_t)((rx_messsage.data[2] << 8) | rx_messsage.data[3]);
            message[receive_id - 1].current = (int16_t)((rx_messsage.data[4] << 8) | rx_messsage.data[5]);
            message[receive_id - 1].temperature = rx_messsage.data[6];
        }
    }
    //   Serial.printf("angle1: %d\t, angle2: %d\n", message[0].angle, message[1].angle);
    //Serial.printf("velocity1: %d\t, velocity2: %d\n", message[0].speed, message[1].speed);
    // Serial.printf("current1: %d\t, current2: %d\n", message[0].current, message[1].current);
}

// volatile int16_t *pid_control(float set_angle)

void pid_control()
{

    int16_t velocity[2];
    velocity[0] = PID_calc(&pidPositionStruct, (float)message[0].angle, angle_ch0+180);     //set_angle[0] / 360.0f * 8191.0f  angle_ch0
    current[0] = PID_calc(&pidVelcityStruct, message[0].speed, velocity[0]);
    // Serial.printf("\ncurrent 0: %d\t  velocity 0: %d\n", current[0], velocity[0]);
    

    velocity[1] = PID_calc(&pidPositionStruct, (float)message[1].angle,  angle_ch1); // set_angle[1] / 360.0f * 8191.0f    angle_ch1+180
    current[1] = PID_calc(&pidVelcityStruct, message[1].speed, velocity[1]);
    // Serial.printf("\ncurrent 1: %d\t  velocity 1: %d\n", current[1], velocity[1]);

    // return current + receive_id;
}
