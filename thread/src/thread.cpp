#include <Arduino.h>
#include "motor.h"
#include "data.h"
#include "thread.h"
#include "pid.h"

// #include "driver/can.h"
// #include "driver/gpio.h"

TaskHandle_t hControlThread;
TaskHandle_t hReciverThread;
TaskHandle_t hControlThread_1;

extern RC_Ctl_t RC_CtrlData;
extern float angle_ch0;
extern float angle_ch1;

void CreateThread()
{
	xTaskCreatePinnedToCore(
		DR16_task_thread,	// TAsk Function
		"DR16_task_thread", // String with name of task
		2048,				// Stack size in bytes
		NULL,				// Parameter passed as input of task
		5,					// Priority of task
		&hReciverThread,	// TAsk handle
		ARDUINO_EVENT_RUNNING_CORE);

	// xTaskCreatePinnedToCore(
	// 	Motor_thread1,		 // TAsk Function
	// 	"motor_ctl_thread2", // String with name of task
	// 	2048,			 // Stack size in bytes
	// 	NULL,				 // Parameter passed as input of task
	// 	9,					 // Priority of task
	// 	&hControlThread_1,	 // TAsk handle
	// 	ARDUINO_RUNNING_CORE);
}

void DR16_task_thread(void *pvParam)
{
	float angle[2];
	for (;;)
	{
		// angle[0] = angle_ch0 + 90.0f;
		// angle[1] = angle_ch1 + 90.0f;

		pid_control();
		transmit_information(0x1ff);

		RemoteDataProcess();
		DR16_control_motor();
		vTaskDelay(5);
	}
}

// void Motor_thread1(void *pvParam)
// {
// 	float angle[2];

// 	for (;;)
// 	{
// 		angle[0] = angle_ch0 + 180.0f;
// 		angle[1] = angle_ch1 + 180.0f;

// 		pid_control(angle);
// 		//transmit_information(0x1ff, current+receive_id-1);

// 		vTaskDelay(1);
// 	}
// }