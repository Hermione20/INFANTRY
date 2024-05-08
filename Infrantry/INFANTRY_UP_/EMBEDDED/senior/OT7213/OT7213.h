#ifndef _OT7213_H_
#define _OT7213_H_
#include "public.h"

typedef enum
{
	OT_POS_MODE      	=0xfb,//λ�ñջ�ģʽ(λ������˫�ջ�)
	OT_SPEED_MODE    	=0xfa,//�ٶȱջ�ģʽ(�ٶ�����˫�ջ�ģʽ)
	OT_CURRENT_MODE  	=0xf9,//���رջ�ģʽ
	OT_P_S_MODE      	=0x00,//�ٶ�λ�ÿ���ģʽ(��λ�ÿ���ģ���ٶȿ���,���������ϴ�������غͶ����½�ʱʱʹ��)
	OT_POS_3LOOP_MODE	=0xf8,//λ���ٶ��������ջ�ģʽ
	OT_MIT_MODE       =0x01,//λ������˫�ջ�ģʽ(ȥ���ٶȻ��Ļ��ֿ���,��ߵ��������Ӧ,��������λ���ڸ�������´��ھ���)
}OT_Mode_e;

typedef enum
{
	OT_POWER_ON      =0xfc,
	OT_POWER_OFF     =0xfd,
	OT_SET_ZERO      =0xfe,
}OT_Special_Instruction_e;

typedef struct
{
	u8 OT_ID;
	float OT_Speed;
	float OT_Pos;
	
	float OT_Torque;
	OT_Mode_e	OT_Mode;
}OT7213_t;

#define OT_Speed_MAX    3361.47//dps
#define OT_Pos_MAX      360//��
#define OT_T_MAX        4//A


extern OT7213_t OT_Test;

u16 OT_Mapping(float _Value,float _Max_Value,u8 len);
float OT_Inverse_Mapping(u16 _Value,float _Max_Value,u8 len);

void OT_Mode_Set(CAN_TypeDef *CANx,u8 ID,OT_Mode_e _OT_Mode);
void OT_Special_Instruction(CAN_TypeDef *CANx,u8 ID,
	OT_Special_Instruction_e _OT_Special_Instruction);
void OT_Control(CAN_TypeDef *CANx,u8 ID,
	float Pos,float Speed,float T,
		u16 Kp,u16 Kd);
void OT_Data_Process(CanRxMsg *msg,u8 ID,OT7213_t* _OT);
float OT_Position_Increment_Mode(CAN_TypeDef *CANx,u8 ID);
void OT_T_Control(CAN_TypeDef *CANx,u8 ID,float T);
	float Float_Limit(float Val,float Thresholds);

#endif/*_OT7213_H_*/


