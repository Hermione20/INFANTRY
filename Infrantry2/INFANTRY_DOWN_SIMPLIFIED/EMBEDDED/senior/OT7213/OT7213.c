#include "OT7213.h"

/**************************↓注意事项↓***************************/
//1、OT7213顺时针为正
//2、位置增量模式下角度发聩才能正常使用
//3、开启位置增量模式向电机发送一帧扩展帧就行了
//4、标准帧才能正常控制电机
//5、电机的一些相关配置只能通过上位机更改，网址https://www.acmemach.cn/#/aboutus
/*************************↑注意事项↑***************************/

/**********************
*@Brief:OT电机映射函数
*@Call:内部或外部
*@Param:
*@Note:将我们的设定值映射到电机控制范围内
*@RetVal:无
**********************/
u16 OT_Mapping(float _Value,float _Max_Value,u8 len)
{
	int static Medium;
	Medium=(pow(2,len))/2;
	
	return (_Value/_Max_Value)*Medium+Medium;
}
/**********************
*@Brief:OT电机逆映射函数
*@Call:内部或外部
*@Param:
*@Note:将电机反馈置映射到我们的单位下
*@RetVal:无
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
	
	while((CANx->TSR&CAN_TSR_TME)==0);//等待发送邮箱空闲
	CAN_Transmit(CANx,&CanxTxMsg);
}

/**********************
*@Brief:OT电机模式设置函数
*@Call:内部或外部
*@Param:
*@Note:无
*@RetVal:无
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
	
	while((CANx->TSR&CAN_TSR_TME)==0);//等待发送邮箱空闲
	CAN_Transmit(CANx,&CanxTxMsg);
}	
/**********************
*@Brief:OT特殊指令函数
*@Call:内部或外部
*@Param:
*@Note:无
*@RetVal:无
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
	
	while((CANx->TSR&CAN_TSR_TME)==0);//等待发送邮箱空闲
	CAN_Transmit(CANx,&CanxTxMsg);
}
/**********************
*@Brief:OT总控制函数
*@Call:内部或外部
*@Param:
*@Note:Kp,Kd,只作用于位置环
*@RetVal:无
**********************/
void OT_Control(CAN_TypeDef *CANx,u8 ID,
	float Pos,float Speed,float T,
		u16 Kp,u16 Kd)
	{
		u16 Pos_Medium,Speed_Medium,T_Medium;
		/**************************↓限幅↓***************************/
		Pos=Float_Limit(Pos,OT_Pos_MAX);
		Speed=Float_Limit(Speed,OT_Speed_MAX);
		T=Float_Limit(T,OT_T_MAX);
		/*************************↑限幅↑***************************/
			
		/**************************↓映射↓***************************/
		Pos_Medium=OT_Mapping(Pos,OT_Pos_MAX,16);
		Speed_Medium=OT_Mapping(Speed,OT_Speed_MAX,12);
		T_Medium=OT_Mapping(T,OT_T_MAX,12);
		/*************************↑映射↑***************************/
		CanTxMsg CanxTxMsg;
		CanxTxMsg.StdId = ID;
		CanxTxMsg.ExtId = ID;
		CanxTxMsg.IDE = CAN_Id_Standard;
		CanxTxMsg.RTR = CAN_RTR_Data;
		CanxTxMsg.DLC = 0x08;
		
		/**************************↓赋值↓***************************/
		CanxTxMsg.Data[0]=Pos_Medium>>8;
		CanxTxMsg.Data[1]=(u8)Pos_Medium;
		CanxTxMsg.Data[2]=Speed_Medium>>4;
		CanxTxMsg.Data[3]=((Speed_Medium<<4)&0xf0)|((Kp>>8)&0x0f);
		CanxTxMsg.Data[4]=Kp;
		CanxTxMsg.Data[5]=(Kd>>4)&0xff;
		CanxTxMsg.Data[6]=((Kd<<4)&0xf0)|((T_Medium>>8)&0x0f);
		CanxTxMsg.Data[7]=(u8)T_Medium;
		/*************************↑赋值↑***************************/
	
		while((CANx->TSR&CAN_TSR_TME)==0);//等待发送邮箱空闲
		CAN_Transmit(CANx,&CanxTxMsg);
	}
	
	/**********************
*@Brief:OT扭矩控制函数
*@Call:内部或外部
*@Param:
*@Note:
*@RetVal:无
**********************/
void OT_T_Control(CAN_TypeDef *CANx,u8 ID,float T)
	{
		u16 T_Medium;
		/**************************↓限幅↓***************************/
		//该电机顺时针为正，加负号使逆时针为正方向
		T=Float_Limit(-T,OT_T_MAX);
		/*************************↑限幅↑***************************/
			
		/**************************↓映射↓***************************/
		T_Medium=OT_Mapping(T,OT_T_MAX,12);
		/*************************↑映射↑***************************/
		CanTxMsg CanxTxMsg;
		CanxTxMsg.StdId = ID;
		CanxTxMsg.ExtId = ID;
		CanxTxMsg.IDE = CAN_Id_Standard;
		CanxTxMsg.RTR = CAN_RTR_Data;
		CanxTxMsg.DLC = 0x08;
		
		/**************************↓赋值↓***************************/
		CanxTxMsg.Data[0]=0;
		CanxTxMsg.Data[1]=0;
		CanxTxMsg.Data[2]=0;
		CanxTxMsg.Data[3]=0;
		CanxTxMsg.Data[4]=0;
		CanxTxMsg.Data[5]=0;
		CanxTxMsg.Data[6]=((T_Medium>>8)&0x0f);
		CanxTxMsg.Data[7]=(u8)T_Medium;
		/*************************↑赋值↑***************************/
	
		while((CANx->TSR&CAN_TSR_TME)==0);//等待发送邮箱空闲
		CAN_Transmit(CANx,&CanxTxMsg);
	}

/**********************
*@Brief:OT电机数据接收处理函数
*@Call:内部或外部
*@Param:
*@Note:无
*@RetVal:无
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
		//逆转方向
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