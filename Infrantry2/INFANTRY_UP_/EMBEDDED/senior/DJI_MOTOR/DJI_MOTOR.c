#include "DJI_MOTOR.h"


/**
  ******************************************************************************
  * @file    DJI_MOTOR.c
  * @author  Lee_ZEKAI
  * @version V1.1.0
  * @date    03-October-2023
  * @brief   ���ļ���д��DJI���ͺŵ���Ľ��㣬����c610���
						 c620�����GM6020����Ľ��㣬��ڲ�������ͨ��
						 �������ṹ�壬can���ߵļ�����can�ṹ�壬��GM
						 6020��������������ʼֵ�趨��
						 
@verbatim
 ===============================================================================
 **/
 
/*****************************dji encoder*************************************/

void GetEncoderBias(volatile Encoder *v, CanRxMsg * msg)
{

            v->ecd_bias = (msg->Data[0]<<8)|msg->Data[1];  //�����ʼ������ֵ��Ϊƫ��  
            v->ecd_value = v->ecd_bias;
            v->last_raw_value = v->ecd_bias;
            v->temp_count++;
}

void EncoderProcess(volatile Encoder *v, CanRxMsg * msg)
{
	int i=0;
	int32_t temp_sum = 0;    
	v->last_raw_value = v->raw_value;
	v->raw_value = (msg->Data[0]<<8)|msg->Data[1];
	v->diff = v->raw_value - v->last_raw_value;
	if(v->diff < -4096)    //���α������ķ���ֵ���̫�󣬱�ʾȦ�������˸ı�
	{
		v->round_cnt++;
		v->ecd_raw_rate = v->diff + 8192;
	}
	else if(v->diff>4096)
	{
		v->round_cnt--;
		v->ecd_raw_rate = v->diff- 8192;
	}		
	else
	{
		v->ecd_raw_rate = v->diff;
	}
	//����õ������ı��������ֵ
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//����õ��Ƕ�ֵ����Χ���������
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*0.04394531f + v->round_cnt * 360;
	v->rate_buf[v->buf_count++] = v->ecd_raw_rate;
	if(v->buf_count == RATE_BUF_SIZE)
	{
		v->buf_count = 0;
	}
	//�����ٶ�ƽ��ֵ
	for(i = 0;i < RATE_BUF_SIZE; i++)
	{
		temp_sum += v->rate_buf[i];
	}
	v->filter_rate = (int32_t)(temp_sum/RATE_BUF_SIZE);		
	v->rate_rpm = (msg->Data[2]<<8)|msg->Data[3];
	v->torque_current = (msg->Data[4]<<8)|msg->Data[5];
}

void GM6020EncoderProcess(volatile Encoder *v, CanRxMsg * msg)
{
	v->last_raw_value = v->raw_value;
	v->raw_value = (msg->Data[0]<<8)|msg->Data[1];
	v->diff = v->raw_value - v->last_raw_value;
	if(v->diff < -4096)    //���α������ķ���ֵ���̫�󣬱�ʾȦ�������˸ı�
	{
		v->round_cnt++;
		v->ecd_raw_rate = v->diff + 8192;
	}
	else if(v->diff>4096)
	{
		v->round_cnt--;
		v->ecd_raw_rate = v->diff- 8192;
	}		
	else
	{
		v->ecd_raw_rate = v->diff;
	}
	v->ecd_value = v->raw_value + v->round_cnt * 8192;
	//����õ��Ƕ�ֵ����Χ���������
	v->ecd_angle = (float)(v->raw_value - v->ecd_bias)*0.0439453125f  + v->round_cnt * 360;
	v->filter_rate = (int16_t)(msg->Data[2]<<8)|msg->Data[3];

	v->temperature = msg->Data[6];
}


void M3508orM2006EncoderTask(volatile Encoder *v, CanRxMsg * msg)
{
	v->can_cnt++;
	(v->can_cnt<=5)?GetEncoderBias(v,msg):EncoderProcess(v,msg);
}


void GM6020EncoderTask(volatile Encoder *v, CanRxMsg * msg,int offset)
{
	v->can_cnt++;
	if(v->can_cnt<=2){v->ecd_bias = offset;}
	GM6020EncoderProcess(v, msg);
	// �����м�ֵ�趨Ҳ��Ҫ�޸�
	if (v->can_cnt <= 10)
	{
		if ((v->ecd_bias - v->ecd_value) < -4000)
		{
				v->ecd_bias = offset + 8192;
		}
		else if ((v->ecd_bias - v->ecd_value) > 4000)
		{
				v->ecd_bias = offset - 8192;
		}
	}
}

void Set_GM6020_IQ1(CAN_TypeDef *CANx, int16_t motor1_iq, int16_t motor2_iq, int16_t motor3_iq, int16_t motor4_iq)
{
	if(abs(motor1_iq)>29000)
		motor1_iq=29000*motor1_iq/abs(motor1_iq);
	if(abs(motor2_iq)>29000)
		motor2_iq=29000*motor2_iq/abs(motor2_iq);
	if(abs(motor3_iq)>29000)
		motor3_iq=29000*motor3_iq/abs(motor3_iq);
	if(abs(motor4_iq)>29000)
		motor4_iq=29000*motor4_iq/abs(motor4_iq);
    CanTxMsg tx_message;
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(motor1_iq >> 8);
    tx_message.Data[1] = (uint8_t)motor1_iq;
    tx_message.Data[2] = (uint8_t)(motor2_iq >> 8);
    tx_message.Data[3] = (uint8_t)motor2_iq;
    tx_message.Data[4] = (uint8_t)(motor3_iq >> 8);
    tx_message.Data[5] = (uint8_t)motor3_iq;
    tx_message.Data[6] = (uint8_t)(motor4_iq >> 8);
    tx_message.Data[7] = (uint8_t)motor4_iq;
    CAN_Transmit(CANx,&tx_message);
}

void Set_GM6020_IQ2(CAN_TypeDef *CANx, int16_t motor5_iq, int16_t motor6iq, int16_t motor7_iq, int16_t motor8_iq)
{
    CanTxMsg tx_message;
    tx_message.StdId = 0x2FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(motor5_iq >> 8);
    tx_message.Data[1] = (uint8_t)motor5_iq;
    tx_message.Data[2] = (uint8_t)(motor6iq >> 8);
    tx_message.Data[3] = (uint8_t)motor6iq;
    tx_message.Data[4] = (uint8_t)(motor7_iq >> 8);
    tx_message.Data[5] = (uint8_t)motor7_iq;
    tx_message.Data[6] = (uint8_t)(motor8_iq >> 8);
    tx_message.Data[7] = (uint8_t)motor8_iq;
    CAN_Transmit(CANx,&tx_message);
}

void Set_C620andC610_IQ1(CAN_TypeDef *CANx, int16_t motor1_iq, int16_t motor2_iq, int16_t motor3_iq, int16_t motor4_iq)
{
	if(abs(motor1_iq)>29000)
		motor1_iq=29000*motor1_iq/abs(motor1_iq);
	if(abs(motor2_iq)>29000)
		motor2_iq=29000*motor2_iq/abs(motor2_iq);
	if(abs(motor3_iq)>29000)
		motor3_iq=29000*motor3_iq/abs(motor3_iq);
	if(abs(motor4_iq)>29000)
		motor4_iq=29000*motor4_iq/abs(motor4_iq);
    CanTxMsg tx_message;
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(motor1_iq >> 8);
    tx_message.Data[1] = (uint8_t)motor1_iq;
    tx_message.Data[2] = (uint8_t)(motor2_iq >> 8);
    tx_message.Data[3] = (uint8_t)motor2_iq;
    tx_message.Data[4] = (uint8_t)(motor3_iq >> 8);
    tx_message.Data[5] = (uint8_t)motor3_iq;
    tx_message.Data[6] = (uint8_t)(motor4_iq >> 8);
    tx_message.Data[7] = (uint8_t)motor4_iq;
    CAN_Transmit(CANx,&tx_message);
}

void Set_C620andC610_IQ2(CAN_TypeDef *CANx, int16_t motor5_iq, int16_t motor6_iq, int16_t motor7_iq, int16_t motor8_iq)
{
    CanTxMsg tx_message;
    tx_message.StdId = 0x1FF;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    tx_message.Data[0] = (uint8_t)(motor5_iq >> 8);
    tx_message.Data[1] = (uint8_t)motor5_iq;
    tx_message.Data[2] = (uint8_t)(motor6_iq >> 8);
    tx_message.Data[3] = (uint8_t)motor6_iq;
    tx_message.Data[4] = (uint8_t)(motor7_iq >> 8);
    tx_message.Data[5] = (uint8_t)motor7_iq;
    tx_message.Data[6] = (uint8_t)(motor8_iq >> 8);
    tx_message.Data[7] = (uint8_t)motor8_iq;
    CAN_Transmit(CANx,&tx_message);
}