#ifndef __CONTROL_TASK_H
#define __CONTROL_TASK_H
#include "public.h"







void control_task(void);
void control_task_Init(void);

extern uint8_t UART4_DMA_TX_BUF[100];
extern int time_tick;
extern int16_t test_v;





#endif

