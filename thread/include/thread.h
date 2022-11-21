#ifndef THREAD_H
#define THREAD_H

#include "data.h"

extern void CreateThread();
extern void DR16_task_thread(void *pvParam);
extern void Motor_thread(void *pvParam);
extern void Motor_thread1(void *pvParam);

#endif