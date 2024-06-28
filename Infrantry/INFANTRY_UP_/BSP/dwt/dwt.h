#ifndef _BSP_DWT_H
#define _BSP_DWT_H

#include "public.h"

/*��������ʱ���Ĵ�������START��END֮�䣬����ʱ������DWT_Cnt.period�ڣ���ȷ��ns*/
#define START 		do{DWT_Cnt.start=DWT->CYCCNT;}while(0);
#define END   	  do{DWT_Cnt.end=DWT->CYCCNT;DWT_Cnt.period=(DWT_Cnt.end-DWT_Cnt.start)/1.68f;}while(0);

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
