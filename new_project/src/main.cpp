#include <Arduino.h>
#include "motor.h"
#include "driver/can.h"
#include "driver/gpio.h"



// void transmit_information(int16_t ID, int16_t current)
// {
//     can_message_t tx_message;
//     tx_message.identifier = ID;
//     tx_message.flags = CAN_MSG_FLAG_NONE;
//     tx_message.data_length_code = 8;
//     for(int i = 0; i<4; i++)
//     {
//         tx_message.data[i*2] = current >> 8;
//         tx_message.data[i*2+1] = current;
//     }
    
//     can_transmit(&tx_message,1);
// }

void setup() {
  // put your setup code here, to run once:
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_33,GPIO_NUM_35,CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  can_driver_install(&g_config,&t_config, &f_config);
  can_start();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
   int16_t current;
   current = pid_control(45);
   transmit_information(0x1ff, current);
   receive_information();
   vTaskDelay(2);
}