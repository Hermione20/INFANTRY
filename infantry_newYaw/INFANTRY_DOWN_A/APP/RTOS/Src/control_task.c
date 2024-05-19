#include "control_task.h"

int time_tick=0;
uint32_t start, end, run_time;

pid_t pid_para_spd;
pid_t pid_para_pos;
float ref;

void control_task(void)
{
	time_tick++;

	if(time_tick%2 == 0)
	{ 
//		TIM_Cmd(TIM4, ENABLE);
//		start = TIM4->CNT;
		chassis_task(); // 200us
//		run_time = TIM4->CNT - start;
//		TIM_Cmd(TIM4, DISABLE);
		
	}	
	
	if(time_tick%3 == 0)
	{
//        Set_GM6020_IQ1(CAN2,0,0,0,0); // 2us
		Set_GM6020_IQ1(CAN2,chassis.voltage[0],chassis.voltage[1],chassis.voltage[2],chassis.voltage[3]); // 2us
    Set_GM6020_IQ1(CAN1, 0 ,0 ,0, chassis.voltage[3]);
	}
	
	if(time_tick%2 == 1)
	{
		Set_C620andC610_IQ1(CAN2,chassis.current[0],chassis.current[1],chassis.current[2],chassis.current[3]);
	}
	
	    if(time_tick%20 == 0)
    { 
			POWER_Control(&Super_Cap_Send);
		}
	
    if(time_tick%10 == 0)
    { 

//			CAN_POWER_Control(CAN1,&Super_Cap_Send);
			Uart_down_to_up_send(&uart_cap_down_data);
    } 
	if(time_tick%1000 == 0)
    { 
		LED1_ON;
		LED0_ON;
    } 
	if(time_tick%2000 == 1)
    { 
      LED1_OFF;
//			LED0_OFF;
    } 
}


void control_task_init()
{		
	time_tick = 0;
	chassis_param_init();
}


//    if(time_tick & 1)
//    {
//        TIM_Cmd(TIM4, ENABLE);
//        start = TIM4->CNT;
//    }
//    else{
//        run_time = TIM4->CNT - start;
//        TIM_Cmd(TIM4, DISABLE);
//    }



