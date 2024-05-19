#include "CanBus.h"


/**
  ******************************************************************************
  * @file    CanBus.c
  * @author  Lee_ZEKAI
  * @version V1.1.0
  * @date    03-October-2023
  * @brief   ���ļ���������can���ߵķ������������������ģ��id��ȥͷ�ļ�����
						 
	* @notice  �й�can��ģ��Ľ��պ�����ʹ�����Ʋ���can_bus.c�ļ��в������еĽ���
						 ������ѡ��ģ��id�����ý��㺯����Ҳ����can_bus.h�ļ����޸�ģ��id
						 �й�can�ķ��ͺ�����ʹ��Ҳ�Ʋ���can_bus.c�ļ��������ܷ�����������
						 ����Ҫ���͵ĺ���������ؽ�can_bus_send_task�����ĵ��÷���controltask
						 �С�
@verbatim
 ===============================================================================
 **/
 
 

void Can1ReceiveMsgProcess(CanRxMsg * msg)
{
    switch (msg->StdId)
    {
			case LEFT_FRICTION:
			{
				M3508orM2006EncoderTask(&general_friction.left_motor,msg);
			}
			break;
			case RIGHT_FRICTION:
			{
				M3508orM2006EncoderTask(&general_friction.right_motor,msg);
			}
			break;
			case POKE:
			{
				M3508orM2006EncoderTask(&general_poke.poke,msg);
			}
			break;
//			case BULLET_HATCH:
//			{
//				M3508orM2006EncoderTask(&bullet_hatch_encoder,msg);
//			}
//			break;
    default:
        break;
    }
}

void Can2ReceiveMsgProcess(CanRxMsg * msg)
{
		{OT_Data_Process(msg,1,&OT_yaw_Encoder);}
		
    switch (msg->StdId)
    {
			
			case GIMBAL_PITCH_MOTOR:
		  {GM6020EncoderTask(&Pitch_Encoder,msg,GMPitchEncoder_Offset);}
				break;
			
        /* code */
    default:
        break;
    }
}


float test_a=0;

void can_bus_send_task(void)
{
	OT_T_Control(CAN2,1,gimbal_data.gim_ref_and_fdb.yaw_motor_input);//
	Set_GM6020_IQ1(CAN2,gimbal_data.gim_ref_and_fdb.yaw_motor_input,gimbal_data.gim_ref_and_fdb.pitch_motor_input,0,0);
	Set_C620andC610_IQ1(CAN1,shoot.fric_current[1],shoot.poke_current[0],shoot.fric_current[0],bullet_hatch.hatch_ref_and_fdb.bullet_hatch_motor_input);//
}
