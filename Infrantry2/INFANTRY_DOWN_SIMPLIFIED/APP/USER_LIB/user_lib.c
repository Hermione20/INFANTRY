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
RampFunc_t Test_Ramp					=RAMP_FUNC_DAFAULT;
RampFunc_t ChassisSpeedRamp0=RAMP_FUNC_DAFAULT;
RampFunc_t ChassisSpeedRamp1=RAMP_FUNC_DAFAULT;
RampFunc_t ChassisSpeedRamp2=RAMP_FUNC_DAFAULT;
RampFunc_t ChassisSpeedRamp3=RAMP_FUNC_DAFAULT;



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
  * @brief          б��������ʼ��
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @param[in]      ���ֵ
  * @param[in]      ��Сֵ
  * @retval         ���ؿ�
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
  * @brief          б���������㣬���������ֵ���е��ӣ� ���뵥λΪ /s ��һ������������ֵ
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      ����ֵ
  * @retval         ���ؿ�
  */
float ramp_calc(RampFunc_t *ramp, float input,float max, float min)
{
    ramp->input = input;
    ramp->out += ramp->input * ramp->frame_period;

			/*�����޷�(����input��������ȫ�޷�)*/
		ramp->max_value = max;
		ramp->min_value = min;
		if(ramp->input>0)
		{
			if (ramp->out > ramp->max_value)
			{ ramp->out = ramp->max_value;}
		}
		else
		{
			if (ramp->out < ramp->min_value)
			{ ramp->out = ramp->min_value;}
		}
			return ramp ->out;
}

/**********************
*@Brief:�����źŷ�����
*@Call:�ڲ����ⲿ
*@Param:T:������������
				f�������ź�Ƶ��
*@Note:��Χ-1~1
*@RetVal:��
**********************/
float Sinusoidal_Waveform_Generator(float T,float f)
{
	float static t,CNT,Value,w;
	
	CNT++;
	
	w=2*PI*f;
	
	t=CNT*T;//ʱ��
	Value=sin(w*t);
	
	if(w*t>=2*PI)
		t=0;
	
	return Value;
}