#ifndef _BSP_DWT_H
#define _BSP_DWT_H

#include "public.h"

/*待测运行时长的代码置于START与END之间，运行时长存于DWT_Cnt.period内，精确到ns*/
#define START 		do{DWT_Cnt.start=DWT->CYCCNT;}while(0);
#define END   	  do{DWT_Cnt.end=DWT->CYCCNT;DWT_Cnt.period=(DWT_Cnt.end-DWT_Cnt.start)/168000000.0f*1000;}while(0);

#define DWT_START_CNT(assist_cnt)   do{assist_cnt = DWT->CYCCNT;}while(0);
#define DWT_END_CNT(assist_cnt)     ((DWT->CYCCNT-assist_cnt)/168000000*1000)
/*在循环体中，记一次开始计数的点，可获取往后计数的时间进行逻辑判断*/
#define DELAY_EXE(control_cnt,Delay_ms,statements) do{EXE_ONCE(control_cnt,DWT_START_CNT(control_cnt)) \
																														if(DWT_END_CNT(control_cnt)<=(Delay_ms/168000000*1000)){statements} \
																														}while(0);

typedef struct 
{
  uint32_t start;
	uint32_t end;
	float     period;
}DWT_Cnt_t;

typedef struct
{
    uint32_t s;
    uint16_t ms;
    uint16_t us;
} DWT_Time_t;

void DWT_Init(uint32_t CPU_Freq_MHz);
float DWT_GetDeltaT(uint32_t *cnt_last);
double DWT_GetDeltaT64(uint32_t *cnt_last);
float DWT_GetTimeline_s(void);
float DWT_GetTimeline_ms(void);
uint64_t DWT_GetTimeline_us(void);
void DWT_Delay(float Delay);
void DWT_SysTimeUpdate(void);

extern DWT_Time_t SysTime;
extern DWT_Cnt_t  DWT_Cnt;
#endif /* BSP_DWT_H_ */
