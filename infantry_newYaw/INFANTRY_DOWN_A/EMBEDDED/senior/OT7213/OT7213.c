#include "OT7213.h"

/**************************��ע�������***************************/
//1��OT7213˳ʱ��Ϊ��
//2��λ������ģʽ�½Ƕȷ�����������ʹ��
//3������λ������ģʽ��������һ֡��չ֡������
//4����׼֡�����������Ƶ��
//5�������һЩ�������ֻ��ͨ����λ�����ģ���ַhttps://www.acmemach.cn/#/aboutus
/*************************��ע�������***************************/

/**********************
*@Brief:OT���ӳ�亯��
*@Call:�ڲ����ⲿ
*@Param:
*@Note:�����ǵ��趨ֵӳ�䵽������Ʒ�Χ��
*@RetVal:��
**********************/
u16 OT_Mapping(float _Value,float _Max_Value,u8 len)
{
	int static Medium;
	Medium=(pow(2,len))/2;
	
	return (_Value/_Max_Value)*Medium+Medium;
}
/**********************
*@Brief:OT�����ӳ�亯��
*@Call:�ڲ����ⲿ
*@Param:
*@Note:�����������ӳ�䵽���ǵĵ�λ��
*@RetVal:��
**********************/
float OT_Inverse_Mapping(u16 _Value,float _Max_Value,u8 len)
{
	int static Medium;
	Medium=(pow(2,len))/2;
	
	return ((float)(_Value-Medium)/(float)Medium)*_Max_Value;
}

float OT_Position_Increment_Mode(CAN_TypeDef *CANx,u8 ID)
{
	CanTxMsg CanxTxMsg;
	CanxTxMsg.ExtId = ID;
	CanxTxMsg.IDE = CAN_Id_Extended;
	CanxTxMsg.RTR = CAN_RTR_Data;
	CanxTxMsg.DLC = 0x08;
	
	while((CANx->TSR&CAN_TSR_TME)==0);//�ȴ������������
	CAN_Transmit(CANx,&CanxTxMsg);
}

/**********************
*@Brief:OT���ģʽ���ú���
*@Call:�ڲ����ⲿ
*@Param:
*@Note:��
*@RetVal:��
**********************/
void OT_Mode_Set(CAN_TypeDef *CANx,u8 ID,OT_Mode_e _OT_Mode)
{
	CanTxMsg CanxTxMsg;
	CanxTxMsg.StdId = ID;
	CanxTxMsg.IDE = CAN_Id_Standard;
	CanxTxMsg.RTR = CAN_RTR_Data;
	CanxTxMsg.DLC = 0x08;
	for(int i=0;i<7;i++)
		CanxTxMsg.Data[i]=0xff;
	switch (_OT_Mode)
	{
		case OT_POS_MODE:
		{
			CanxTxMsg.Data[7]=0xfb;
		}break;
		
		case OT_SPEED_MODE:
		{
			CanxTxMsg.Data[7]=0xfa;
		}break;
			
		case OT_CURRENT_MODE:
		{
			CanxTxMsg.Data[7]=0xf9;
		}break;
		
		case OT_P_S_MODE:
		{
			return;
		}break;
		
		case OT_POS_3LOOP_MODE:
		{
			CanxTxMsg.Data[7]=0xf8;
		}break;
			
		case OT_MIT_MODE:
		{
			return;
		}break;
		default:
		{
			return;
		}break;
	}
	
	while((CANx->TSR&CAN_TSR_TME)==0);//�ȴ������������
	CAN_Transmit(CANx,&CanxTxMsg);
}	
/**********************
*@Brief:OT����ָ���
*@Call:�ڲ����ⲿ
*@Param:
*@Note:��
*@RetVal:��
**********************/
void OT_Special_Instruction(CAN_TypeDef *CANx,u8 ID,OT_Special_Instruction_e _OT_Special_Instruction)
{
	CanTxMsg CanxTxMsg;
	CanxTxMsg.StdId = ID;
	CanxTxMsg.ExtId = ID;
	CanxTxMsg.IDE = CAN_Id_Standard;
	CanxTxMsg.RTR = CAN_RTR_Data;
	CanxTxMsg.DLC = 0x08;
	for(int i=0;i<7;i++)
		CanxTxMsg.Data[i]=0xff;
	CanxTxMsg.Data[7]=_OT_Special_Instruction;
	
	while((CANx->TSR&CAN_TSR_TME)==0);//�ȴ������������
	CAN_Transmit(CANx,&CanxTxMsg);
}
/**********************
*@Brief:OT�ܿ��ƺ���
*@Call:�ڲ����ⲿ
*@Param:
*@Note:Kp,Kd,ֻ������λ�û�
*@RetVal:��
**********************/
void OT_Control(CAN_TypeDef *CANx,u8 ID,
	float Pos,float Speed,float T,
		u16 Kp,u16 Kd)
	{
		u16 Pos_Medium,Speed_Medium,T_Medium;
		/**************************���޷���***************************/
		Pos=Float_Limit(Pos,OT_Pos_MAX);
		Speed=Float_Limit(Speed,OT_Speed_MAX);
		T=Float_Limit(T,OT_T_MAX);
		/*************************���޷���***************************/
			
		/**************************��ӳ���***************************/
		Pos_Medium=OT_Mapping(Pos,OT_Pos_MAX,16);
		Speed_Medium=OT_Mapping(Speed,OT_Speed_MAX,12);
		T_Medium=OT_Mapping(T,OT_T_MAX,12);
		/*************************��ӳ���***************************/
		CanTxMsg CanxTxMsg;
		CanxTxMsg.StdId = ID;
		CanxTxMsg.ExtId = ID;
		CanxTxMsg.IDE = CAN_Id_Standard;
		CanxTxMsg.RTR = CAN_RTR_Data;
		CanxTxMsg.DLC = 0x08;
		
		/**************************����ֵ��***************************/
		CanxTxMsg.Data[0]=Pos_Medium>>8;
		CanxTxMsg.Data[1]=(u8)Pos_Medium;
		CanxTxMsg.Data[2]=Speed_Medium>>4;
		CanxTxMsg.Data[3]=((Speed_Medium<<4)&0xf0)|((Kp>>8)&0x0f);
		CanxTxMsg.Data[4]=Kp;
		CanxTxMsg.Data[5]=(Kd>>4)&0xff;
		CanxTxMsg.Data[6]=((Kd<<4)&0xf0)|((T_Medium>>8)&0x0f);
		CanxTxMsg.Data[7]=(u8)T_Medium;
		/*************************����ֵ��***************************/
	
		while((CANx->TSR&CAN_TSR_TME)==0);//�ȴ������������
		CAN_Transmit(CANx,&CanxTxMsg);
	}
	
	/**********************
*@Brief:OTŤ�ؿ��ƺ���
*@Call:�ڲ����ⲿ
*@Param:
*@Note:
*@RetVal:��
**********************/
void OT_T_Control(CAN_TypeDef *CANx,u8 ID,float T)
	{
		u16 T_Medium;
		/**************************���޷���***************************/
		//�õ��˳ʱ��Ϊ�����Ӹ���ʹ��ʱ��Ϊ������
		T=Float_Limit(-T,OT_T_MAX);
		/*************************���޷���***************************/
			
		/**************************��ӳ���***************************/
		T_Medium=OT_Mapping(T,OT_T_MAX,12);
		/*************************��ӳ���***************************/
		CanTxMsg CanxTxMsg;
		CanxTxMsg.StdId = ID;
		CanxTxMsg.ExtId = ID;
		CanxTxMsg.IDE = CAN_Id_Standard;
		CanxTxMsg.RTR = CAN_RTR_Data;
		CanxTxMsg.DLC = 0x08;
		
		/**************************����ֵ��***************************/
		CanxTxMsg.Data[0]=0;
		CanxTxMsg.Data[1]=0;
		CanxTxMsg.Data[2]=0;
		CanxTxMsg.Data[3]=0;
		CanxTxMsg.Data[4]=0;
		CanxTxMsg.Data[5]=0;
		CanxTxMsg.Data[6]=((T_Medium>>8)&0x0f);
		CanxTxMsg.Data[7]=(u8)T_Medium;
		/*************************����ֵ��***************************/
	
		while((CANx->TSR&CAN_TSR_TME)==0);//�ȴ������������
		CAN_Transmit(CANx,&CanxTxMsg);
	}

/**********************
*@Brief:OT������ݽ��մ�����
*@Call:�ڲ����ⲿ
*@Param:
*@Note:��
*@RetVal:��
**********************/
	u16 Speed_Medium,T_Medium;
	int Pos_Medium;
void OT_Data_Process(CanRxMsg *msg,u8 ID,OT7213_t* _OT)
{
	
	if((msg->Data[0])==ID&&msg->DLC==8)
	{
		Pos_Medium=((u32)msg->Data[8]<<24|(u32)msg->Data[7]<<16|(u32)msg->Data[1]<<8|msg->Data[2])&0xfffff;
		Speed_Medium=(msg->Data[3]<<4|msg->Data[4]>>4)+1;
		T_Medium=((msg->Data[4]&0x0f)<<8|msg->Data[5])+1;
		
		_OT->OT_ID=msg->Data[0];
		//��ת����
		_OT->OT_Pos=360-(float)(Pos_Medium)/1048576*360;
		_OT->OT_Speed=-OT_Inverse_Mapping(Speed_Medium,OT_Speed_MAX,12);
		_OT->OT_Torque=-OT_Inverse_Mapping(T_Medium,OT_T_MAX,12);
	}
}


float Float_Limit(float Val,float Thresholds)
{
	if(Val>fabs(Thresholds))
		Val=fabs(Thresholds);
	else if(Val<-fabs(Thresholds))
		Val=-fabs(Thresholds);
	return Val;
}