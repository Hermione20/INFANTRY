#include "control_task.h"


int time_tick = 0;
float auto_aim_rotate_yaw=0;
void control_task(void)
{
	time_tick++;

	buff_karman_filter_calc(&buff_kalman_filter,yaw_angle_ref_aim,pit_angle_ref_aim,&new_location.buff_kf_flag);

	if(time_tick%4==1)
	{
        uart_cha_task(RC_CtrlData.inputmode,
												chassis.follow_gimbal,
                        chassis.chassis_speed_mode,
                        chassis.ctrl_mode,
                        OT_yaw_Encoder.OT_Pos,//0~360
                        OT_yaw_Encoder.OT_Speed,
                        chassis.ChassisSpeed_Ref.left_right_ref,
                        chassis.ChassisSpeed_Ref.forward_back_ref,
                        chassis.ChassisSpeed_Ref.rotate_ref,
                        judge_rece_mesg.power_heat_data.chassis_power,
                        judge_rece_mesg.power_heat_data.chassis_power_buffer,
                        judge_rece_mesg.game_robot_state.chassis_power_limit,
												judge_rece_mesg.game_robot_state.power_management_chassis_output,
												judge_rece_mesg.game_robot_state.robot_level,
												chassis.climbing_mode);
       

	}	
	
	if(time_tick%70==0)
	{ OT_Position_Increment_Mode(CAN2,1);}
	
		 if(time_tick%800==0)
	 {
		 OT_Init_And_Heartbeating(CAN2,1,&OT_yaw_Encoder,OT_POWER_ON);
	 }
	
	
	if(time_tick%2==0)
	{
		  shoot_task();
			bullet_hatch_task();
	}
	if(time_tick%3 == 0)
	{ 
			gimbal_task();
//		OT_T_Control(CAN2,1,1);
			can_bus_send_task();
	}
	

		if(time_tick%5 == 0)
	{

		if(gimbal_data.auto_aim_rotate_flag==1)	
		{
		auto_aim_rotate_yaw = convert_ecd_angle_to__pi_pi(gimbal_gyro.yaw_Angle,auto_aim_rotate_yaw);
		send_protocol(auto_aim_rotate_yaw,-gimbal_gyro.pitch_Angle,gimbal_gyro.roll_Angle,judge_rece_mesg.game_robot_state.robot_id,27,gimbal_data.ctrl_mode,UART4_DMA_TX_BUF);
		}
		else
		{send_protocol(-gimbal_gyro.yaw_Angle,-gimbal_gyro.pitch_Angle,gimbal_gyro.roll_Angle,judge_rece_mesg.game_robot_state.robot_id,27,gimbal_data.ctrl_mode,UART4_DMA_TX_BUF);}
		//send_protocol_New(gimbal_gyro.yaw_Angle,gimbal_gyro.pitch_Angle,gimbal_gyro.roll_Angle,judge_rece_mesg.game_robot_state.robot_id,27,gimbal_data.ctrl_mode,UART4_DMA_TX_BUF); 
	}//
		if(time_tick%100==1)
	{
		Client_send_handle();
	}

}

void control_task_Init(void)
{
		gimbal_parameter_Init();
		shot_param_init();
		bullet_hatch_param_init();
}
