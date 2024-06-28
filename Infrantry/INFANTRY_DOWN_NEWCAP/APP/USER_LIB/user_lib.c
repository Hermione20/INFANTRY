#include "user_lib.h"
/**
  ******************************************************************************
  * @file	 user_lib.c
  * @author  TC
  * @version V1.0.0
  * @date    2024/6/2
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
RampFunc_t ChassisRamp;




#define VAL_LIMIT(val, min, max)\
if(val<=min)\
{\
	val = min;\
}\
else if(val>=max)\
{\
	val = max;\
}\


/**
  * @brief          斜波函数初始化
  * @author         RM
  * @param[in]      斜波函数结构体
  * @param[in]      间隔的时间，单位 s
  * @param[in]      最大值
  * @param[in]      最小值
  * @retval         返回空
  */
void ramp_init(RampFunc_t *ramp, float frame_period, float max, float min)
{
    ramp->frame_period = frame_period;
    ramp->max_value = max;
    ramp->min_value = min;
    ramp->input = 0.0f;
    ramp->out = 0.0f;
}

/**
  * @brief          斜波函数计算，根据输入的值进行叠加， 输入单位为 /s 即一秒后增加输入的值
  * @author         RM
  * @param[in]      斜波函数结构体
  * @param[in]      输入值
  * @retval         返回空
  */
float ramp_calc(RampFunc_t *ramp, float input)
{
    ramp->input = input;
    ramp->out += ramp->input * ramp->frame_period;

		/*上下限幅*/
    if (ramp->out > ramp->max_value)
		{ ramp->out = ramp->max_value;}
    else if (ramp->out < ramp->min_value)
		{ ramp->out = ramp->min_value;}

    return ramp->out;
}


/**********************
*@Brief:正弦信号发生器
*@Call:内部或外部
*@Param:T:函数调用周期
				f：正弦信号频率
*@Note:范围-1~1
*@RetVal:无
**********************/
float Sinusoidal_Waveform_Generator(float T,float f)
{
	float static t,CNT,Value,w;
	
	CNT++;
	
	w=2*PI*f;
	
	t=CNT*T;//时间
	Value=sin(w*t);
	
	if(w*t>=2*PI)
		t=0;
	
	return Value;
}