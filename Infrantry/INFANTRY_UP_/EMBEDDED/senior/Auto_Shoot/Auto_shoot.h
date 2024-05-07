#ifndef __AUTO_SHOOT_H
#define __AUTO_SHOOT_H
#include "public.h"



#define AUTO_StdID_Visual_Need_1  0x300
#define AUTO_StdID_Visual_Need_2  0x301

#define AUTO_StdID_Visual_Fdb_1  0x302
#define AUTO_StdID_Visual_Fdb_2  0x303
#define AUTO_StdID_Visual_Fdb_3  0x304
#define AUTO_StdID_Visual_Fdb_4  0x305

/***********************************autoshoot****************************************/

typedef struct
{
	float Yaw_Angle;//Ŀ��yaw��λ��
	float Pitch_Angle;//Ŀ��Pitch��λ��
	
	float Yaw_Angle_Last;//��һʱ��Ŀ��yaw��λ��
	float Pitch_Angle_Last;//��һʱ��Pitch��λ��
	
	uint8_t Priority;//ʶ��Ŀ�����ȼ�
	uint8_t Attack_Choice;//�����ж�
	uint8_t enable_shoot;
	
	uint8_t  Flag_Get_Target;//Ŀ��������־λ��1������Ŀ�꣬0��δʶ��Ŀ��
	uint16_t Lost_Cnt;//Ŀ�궪ʧ������
}Auto_Aim_t;



typedef struct
{
	int Yaw_Angle;//Ŀ��yaw��λ��
	int Pitch_Angle;//Ŀ��Pitch��λ��
	
	int Yaw_Angle_Last;//��һʱ��Ŀ��yaw��λ��
	int Pitch_Angle_Last;//��һʱ��Pitch��λ��
	
	float Yaw_Speed;//Ŀ��yaw���ٶ�
	float Pitch_Speed;//Ŀ��pitch���ٶ�
	
	uint8_t  Flag_Get_Target;//Ŀ��������־λ��1������Ŀ�꣬0��δʶ��Ŀ��
	uint16_t Lost_Cnt;//Ŀ�궪ʧ������
}Buff_t;


typedef struct
{
	Auto_Aim_t Auto_Aim;
	Buff_t Buff;
}Auto_Shoot_t;

typedef struct
{
	uint8_t Header;
	float Pitch_Angle;
	float Yaw_Angle; 
	uint8_t Priority;
	uint8_t Attack_Choice;
	uint8_t enable_shoot;
	uint16_t Check_Sum;
	uint8_t Tail;
}New_Auto_Aim_t;

typedef struct
{
	float Pitch;
	float Yaw;
	float Roll;
	float Shoot_Speed;
	uint8_t 	Current_Color;//����1����0��
	uint8_t Enemy_Survical_State_1;//��1��������0��
	uint8_t Enemy_Survical_State_2;
	uint8_t Enemy_Survical_State_3;
	uint8_t Enemy_Survical_State_4;
	uint8_t Enemy_Survical_State_5;
	uint8_t Enemy_Survical_State_7;
	uint8_t Enemy_Output;
	uint8_t Another_Priority;
}New_Auto_Aim_Send_t;	

typedef struct
{
	float Yaw_Angle;
	float Pitch_Angle;
	float Roll_Angle;
	uint8_t Robot_ID;
	uint8_t Scan_Flag;
	uint8_t Another_Priority;
}Visual_Data_Need_t;

extern Auto_Shoot_t My_Auto_Shoot;
extern Auto_Shoot_t Othter_Auto_Shoot;
extern New_Auto_Aim_t New_Auto_Aim;
extern New_Auto_Aim_Send_t New_Auto_Aim_Send;
extern Visual_Data_Need_t Visual_Data_Need;

void Vision_Process_General_Message(unsigned char* address, unsigned int length, Auto_Shoot_t *Auto_Shoot);
void Vision_Process_General_Message_New(unsigned char* address, unsigned int length, Auto_Shoot_t *Auto_Shoot);
void send_protocol(float x, float y, float r, int id, float ammo_speed, int gimbal_mode, uint8_t *data);
void send_protocol_New(float Yaw, float Pitch, float Roll, int id, float ammo_speed, int Attack_Engineer_Flag, uint8_t* data);
void AUTO_Shoot_CAN_Send_Handle(CAN_TypeDef *CANx, uint8_t *address_1,uint8_t *address_2,uint8_t *address_3,uint8_t *address_4,uint8_t *address_5);
void AUTO_Shoot_CAN_Send_Handle_2_1(CAN_TypeDef *CANx, uint8_t *address_1);

void AUTO_Shoot_CAN_Rec_Process(CanRxMsg * msg,uint8_t* adress);
void AUTO_Shoot_CAN_Rec_Process_2(CanRxMsg * msg,uint8_t* adress);


#endif

