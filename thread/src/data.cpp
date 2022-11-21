#include <Arduino.h>
#include "data.h"
#include "thread.h"
#include "driver/can.h"
#include "motor.h"
#include "pid.h"

RC_Ctl_t RC_CtrlData;
float angle_ch0 = 0;
float angle_ch1 = 0;
extern motor_message message;

void RemoteDataProcess()
{
    uint8_t pData[18] = {0};

    if (Serial2.available())
    {
        Serial2.readBytes(pData, 18);

        RC_CtrlData.rc.ch0 = (int16_t)(pData[0] | (uint16_t)(pData[1] << 8)) & 0x07FF;
        RC_CtrlData.rc.ch1 = ((int16_t)pData[1] >> 3 | (int16_t)pData[2] << 5) & 0x07FF;
        RC_CtrlData.rc.ch2 = ((int16_t)pData[2] >> 6 | (int16_t)pData[3] << 2 | (int16_t)pData[4] << 10) & 0x07FF;
        RC_CtrlData.rc.ch3 = ((int16_t)pData[4] >> 1 | (int16_t)pData[5] << 7) & 0x07FF;

        RC_CtrlData.rc.s1 = ((int16_t)pData[5] >> 4 & 0x000C) >> 2;
        RC_CtrlData.rc.s2 = ((int16_t)pData[5] & 0x0003);

        RC_CtrlData.mouse.x = ((int16_t)pData[6] | (int16_t)pData[7] << 8);
        RC_CtrlData.mouse.y = ((int16_t)pData[8] | (int16_t)pData[9] << 8);
        RC_CtrlData.mouse.z = ((int16_t)pData[10] | (int16_t)pData[11] << 8);

        RC_CtrlData.mouse.press_l = (int16_t)pData[12];
        RC_CtrlData.mouse.press_r = (int16_t)pData[13];

        RC_CtrlData.key.v = ((int16_t)pData[14] | (int16_t)pData[15] << 8);
         Serial.printf("ch0: %d\t, ch1: %d\n",RC_CtrlData.rc.ch0, RC_CtrlData.rc.ch1);
    }
}

void DR16_control_motor()
{
    angle_ch0 += ((float)(RC_CtrlData.rc.ch0 - 1024) / 660.0f) * 2.0f / 5;             //yaw    rc0
    angle_ch1 += ((float)(RC_CtrlData.rc.ch1 - 1024) / 660.0f) * 2.0f / 5;              //pitch   rc1
    if (angle_ch0 >= 90)
    {
        angle_ch0 = 90;
    }
    else if (angle_ch0 <= -90) {
        angle_ch0 = -90;
    }

     if (angle_ch1 >= 250)
    {
        angle_ch1 = 250;
    }
    else if (angle_ch1 <130) {
        angle_ch1 = 130;
    }
     Serial.printf("set_angle_0:  %f\n",angle_ch0);
     Serial.printf("set_angle_1:  %f\n",angle_ch1);
}
