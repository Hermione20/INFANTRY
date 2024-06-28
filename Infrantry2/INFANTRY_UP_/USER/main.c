#include "main.h"
u32 DWT_temp1=0;

int main()
{

   BSP_Init();
	control_task_Init();
	 
	
	while(1)
	{
		DWT_temp1=DWT->CYCCNT/168000;

	}
}
