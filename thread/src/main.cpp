#include <Arduino.h>
#include "driver/can.h"
#include "driver/gpio.h"
#include "thread.h"
#include "motor.h"
#include "pid.h"

// extern RC_Ctl_t RC_CtrlData;
extern motor_message message[7];
extern int16_t current[2];

pid_type_def pidPositionStruct;
pid_type_def pidVelcityStruct;

void setup()
{
  float yaw_pid_position[3] = {0.4f, 0.00f, 0.0f};
  float yaw_pid_velcity[3] = {110.0f, 1.0f, 0.0f};

  float pitch_pid_position[3] = {0.4f, 0.00f, 0.0f};
  float pitch_pid_velcity[3] = {120.0f, 1.5f, 0.0f};
  const float max_current = 20000;
  const float max_ki_current = 2000;
  const float max_spd = 300;
  const float max_ki_spd = 70;

  Serial.begin(115200);
  Serial2.begin(100000);

  // put your setup code here, to run once:
  can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_33, GPIO_NUM_35, CAN_MODE_NORMAL);
  can_timing_config_t t_config = CAN_TIMING_CONFIG_1MBITS();
  can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();

  PID_init(&pidPositionStruct, PID_POSITION, pitch_pid_position, max_spd, max_ki_spd);
  PID_init(&pidVelcityStruct, PID_POSITION, pitch_pid_velcity, max_current, max_ki_current);

  can_driver_install(&g_config, &t_config, &f_config);
  can_start();

  CreateThread();
}

void loop()
{
  receive_information();
  vTaskDelay(1);
}