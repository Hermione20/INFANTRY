#ifndef __USER_LIB
#define __USER_LIB
#include "public.h"


#ifndef NULL  
#define NULL 0
#endif
#ifndef PI   
#define PI 3.14159265358979f
#endif
#ifndef ANGLE_TO_RAD   //定义 角度(度)转换到 弧度的比例
#define ANGLE_TO_RAD 0.01745329251994329576923690768489f
#endif
#ifndef RAD_TO_ANGLE   //定义 弧度 转换到 角度的比例
#define RAD_TO_ANGLE 57.295779513082320876798154814105f
#endif

typedef struct RampFunc_t
{
    float input;        //输入数据
    float out;          //输出数据
    float min_value;    //限幅最小值
    float max_value;    //限幅最大值
    float frame_period; //时间间隔
		void  (*Init)(struct RampFunc_t *ramp, float frame_period, float max, float min);
	  float (*Calc)(struct RampFunc_t *ramp, float input);
} RampFunc_t;

#define RAMP_FUNC_DAFAULT \
{ \
							.input = 0, \
							.out = 0, \
							.min_value= 0, \
							.max_value=0, \
							.Init = &ramp_init, \
							.Calc = &ramp_calc, \
						} \

float ramp_calc(RampFunc_t *ramp, float input);
float Sinusoidal_Waveform_Generator(float T,float f);
#endif