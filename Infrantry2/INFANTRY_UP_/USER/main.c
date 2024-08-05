#include "main.h"
u32 DWT_temp1=0;

int main()
{

   BSP_Init();//88884		85392
	control_task_Init();//3.27    1.5
	 
	
	while(1)
	{
		DWT_temp1=DWT->CYCCNT/168000;

	}
}
