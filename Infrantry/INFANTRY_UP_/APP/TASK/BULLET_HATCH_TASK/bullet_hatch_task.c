#include "bullet_hatch_task.h"


bullet_hatch_t bullet_hatch;



void bullet_hatch_param_init()
{
	PID_struct_init(&bullet_hatch.pid_hatch_speed_init, POSITION_PID, 10000, 10, 15,0,0); 
	PID_struct_init(&bullet_hatch.pid_hatch_angle_init, POSITION_PID, 300, 10, 4,0,0); 
	PID_struct_init(&bullet_hatch.pid_hatch_speed, 			 POSITION_PID, 10000, 10, 8,0,0);
	PID_struct_init(&bullet_hatch.pid_hatch_angle, 			 POSITION_PID, 300, 10, 4,0,0);
}


void bullet_hatch_task()
{
		
		hatch_state_mode_switch();
	
		if(bullet_hatch.bullet_hatch_init_flag==0)
			{
			bullet_hatch.bullet_hatch_mode=INIT;
			bullet_hatch.hatch_ref_and_fdb.hatch_speed_ref=100;
			bullet_hatch.hatch_ref_and_fdb.hatch_speed_fdb=bullet_hatch_encoder.filter_rate;
			bullet_hatch.hatch_ref_and_fdb.bullet_hatch_motor_input=pid_calc(&bullet_hatch.pid_hatch_speed_init,\
																																								 bullet_hatch.hatch_ref_and_fdb.hatch_speed_fdb,\
																																								 bullet_hatch.hatch_ref_and_fdb.hatch_speed_ref);
			if(fabs(bullet_hatch_encoder.torque_current)>1300&&fabs(bullet_hatch_encoder.filter_rate)<1)
			{
				bullet_hatch.b_cnt++;
				if(bullet_hatch.b_cnt>=1000)
				{				
					bullet_hatch_encoder.ecd_bias=bullet_hatch_encoder.ecd_value;
					bullet_hatch.hatch_ref_and_fdb.bullet_hatch_motor_input=0;
  				bullet_hatch.bullet_hatch_mode=CLOSE;
					bullet_hatch.bullet_hatch_init_flag=1;
				}
			}
			}
			
			
			if(bullet_hatch.bullet_hatch_init_flag==1)
			{	
				
				bullet_hatch.b_cnt=0;
				if(bullet_hatch.bullet_hatch_mode==OPEN)
					bullet_hatch.hatch_ref_and_fdb.hatch_angle_ref=-3500;
				else
					bullet_hatch.hatch_ref_and_fdb.hatch_angle_ref=0;//¿Õ³Ì
				
				bullet_hatch.hatch_ref_and_fdb.hatch_speed_fdb=bullet_hatch_encoder.filter_rate;
				bullet_hatch.hatch_ref_and_fdb.hatch_angle_fdb=bullet_hatch_encoder.ecd_angle;
				bullet_hatch.hatch_ref_and_fdb.bullet_hatch_motor_input=pid_double_loop_cal(&bullet_hatch.pid_hatch_angle,\
																																															 &bullet_hatch.pid_hatch_speed,\
																																															bullet_hatch.hatch_ref_and_fdb.hatch_angle_ref,\
																																															bullet_hatch.hatch_ref_and_fdb.hatch_angle_fdb,\
																																															&bullet_hatch.hatch_ref_and_fdb.hatch_speed_ref,\
																																															bullet_hatch.hatch_ref_and_fdb.hatch_speed_fdb,0);
			}
			
			
			
}
			
void hatch_state_mode_switch()
{
	  static uint16_t key_R_cnt=0;
	 /****************************µ¯²Õ×´Ì¬¸üÐÂ**********************************************/		
		switch(RC_CtrlData.inputmode)
			{
					case REMOTE_INPUT:
				{
					if(RC_CtrlData.rc.ch4>1500)
							bullet_hatch.bullet_hatch_mode=OPEN;
					 else
							bullet_hatch.bullet_hatch_mode=CLOSE;
				}break;
				
					case KEY_MOUSE_INPUT:
				{
	 
					if(RC_CtrlData.Key_Flag.Key_R_TFlag&&bullet_hatch.bullet_hatch_mode==CLOSE)
					{
						bullet_hatch.bullet_hatch_mode=OPEN;
					}
					else if(RC_CtrlData.Key_Flag.Key_R_Flag&&bullet_hatch.bullet_hatch_mode==OPEN)
					{
						key_R_cnt++;
						if(key_R_cnt>=100)
						{
						 bullet_hatch.bullet_hatch_mode=CLOSE;
						 key_R_cnt=0;
						}
					}					
				 }break;

					default:
					{
					}
					break;		
			}
}


			
			
			



















