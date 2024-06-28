#include "17mm_shoot_task.h"

/* Variables_definination-----------------------------------------------------------------------------------------------*/
  shoot_t shoot = {
        .Bullet_Speed_Kalman.X_hat=28,
        .Bullet_Speed_Kalman.Error_Mea=0.2,
        .Bullet_Speed_Kalman.Error_Est=5,
};
  
  
	pid_t pid_trigger_angle ={0};
	pid_t pid_trigger_angle_buf={0};
	pid_t pid_trigger_speed_buf={0};
	pid_t pid_trigger_speed={0};
	pid_t pid_rotate[2]     = {0};
/*----------------------------------------------------------------------------------------------------------------------*/


void shot_param_init()
{

    PID_struct_init(&pid_trigger_angle, POSITION_PID, 6000, 1000,20,0.2,10);//20 0.2 0
	PID_struct_init(&pid_trigger_speed,POSITION_PID,29000,10000,30,0,0);//100 0.1 4

	
	PID_struct_init(&pid_trigger_angle_buf,POSITION_PID, 2000 , 50    ,  15.6, 0.01f  ,0);
	PID_struct_init(&pid_trigger_speed_buf,POSITION_PID,10000 , 5500 ,  20 , 0  , 0 );
	
    PID_struct_init(&pid_rotate[1], POSITION_PID,15500,500,50,0,0);
    PID_struct_init(&pid_rotate[0], POSITION_PID,15500,500,50,0,0);

    shoot.friction_pid.speed_ref[0] =FRICTION_SPEED_30;            //ûװ����ϵͳ������Ƚ���Ħ�����ٶȸ�ֵ
	
    shoot.ctrl_mode=1;
    shoot.limit_heart0=80;
}


float Shooter_Bullet_Speed_Self_Adaptation(float Bullet_Speed)
{
        float static Bullet_Speed_Error;
        Bullet_Speed_Error=(BULLET_SPEED_TARGET-Bullet_Speed);
        
        return Bullet_Speed_Error*BULLET_SPEED_SELF_ADAPTATION_K;
}



void heat_switch()
{
											//+\0.002*shoot.cooling_ratio
	//ʣ�෢����=����������-����ֵ��/10      ��������һ�����������Ϊ10
  shoot.remain_bullets=(judge_rece_mesg.game_robot_state.shooter_barrel_heat_limit-\
										 judge_rece_mesg.power_heat_data.shooter_id1_17mm_cooling_heat)/10;    //ʣ�෢����
  shoot.cooling_ratio=judge_rece_mesg.game_robot_state.shooter_barrel_cooling_value;      //��ȴ����
      
	//������Ƶ
//		 if(shoot.bulletspead_level==1)
//                {
//                    shoot.shoot_frequency=22;
//                }
//    else{
			if(judge_rece_mesg.game_robot_state.robot_level==1)//level_��ȴ
				shoot.shoot_frequency=14;						
			else if(judge_rece_mesg.game_robot_state.robot_level==2)//level_2
				shoot.shoot_frequency=16;
			else if(judge_rece_mesg.game_robot_state.robot_level==3)//level_3
				shoot.shoot_frequency=18;
			else if(judge_rece_mesg.game_robot_state.robot_level==4)
				shoot.shoot_frequency=19;
			else if(judge_rece_mesg.game_robot_state.robot_level==5)
				shoot.shoot_frequency=20;
			else if(judge_rece_mesg.game_robot_state.robot_level==6)
				shoot.shoot_frequency=21;
			else if(judge_rece_mesg.game_robot_state.robot_level==7)
				shoot.shoot_frequency=24;
			else if(judge_rece_mesg.game_robot_state.robot_level==8)
				shoot.shoot_frequency=25;
			else if(judge_rece_mesg.game_robot_state.robot_level==9)
				shoot.shoot_frequency=25;
			else if(judge_rece_mesg.game_robot_state.robot_level==10)
				shoot.shoot_frequency=26;
			else
				shoot.shoot_frequency=19;					
		                    
//         }


						
}

void heat_shoot_frequency_limit()//������Ƶ���Ʋ���
{

		heat_switch();//����ģʽѡ��
//		bullets_spilling();//�õ�

	if(shoot.shoot_frequency!=0)                          //�����Ƶ��Ϊ0����Ƶ�����趨�������ƣ�
	{
		//����2msһ��  time_tick 1ms����һ��
			  switch(judge_rece_mesg.game_robot_state.robot_level)
        {
      case 1:
			{shoot.will_time_shoot=(shoot.remain_bullets-2.7)*1000/shoot.shoot_frequency;}break;
			case 2:
			{shoot.will_time_shoot=(shoot.remain_bullets-2.7)*1000/shoot.shoot_frequency;}break;
			case 3:
			{shoot.will_time_shoot=(shoot.remain_bullets-4)*1000/shoot.shoot_frequency;}break;
			case 4:
			{shoot.will_time_shoot=(shoot.remain_bullets-4.2)*1000/shoot.shoot_frequency;}break;
			case 5:
			{shoot.will_time_shoot=(shoot.remain_bullets-4.4)*1000/shoot.shoot_frequency;}break;
			case 6:
			{shoot.will_time_shoot=(shoot.remain_bullets-4.6)*1000/shoot.shoot_frequency;}break;
			case 7:
			{shoot.will_time_shoot=(shoot.remain_bullets-5.0)*1000/shoot.shoot_frequency;}break;
			case 8:
			{shoot.will_time_shoot=(shoot.remain_bullets-5.8)*1000/shoot.shoot_frequency;}break;
			case 9:
			{shoot.will_time_shoot=(shoot.remain_bullets-6.8)*1000/shoot.shoot_frequency;}break;
			case 10:
			{shoot.will_time_shoot=(shoot.remain_bullets-7.4)*1000/shoot.shoot_frequency;}break;
			default :
			{shoot.will_time_shoot=(shoot.remain_bullets-4)*1000/shoot.shoot_frequency;} break;
        }
	}
}	
	
void bullets_spilling()//������Ƶ���Ʋ���
{
	static 	int fric_run_time=0;
	
	if(shoot.fric_wheel_run==1&&
					 shoot.poke_run==1&&
					 shoot.ctrl_mode==1)
	{ 
		fric_run_time++;
		if(fric_run_time<20&&shoot.remain_bullets>4)           //�տ�ʼ������ʣ�����ܶ�ʱ���������Ƶ     
		{shoot.shoot_frequency=shoot.shoot_frequency*1.2;}	
	}
	else
	{
		fric_run_time=0;
		if(shoot.shoot_frequency!=0)
		{pid_trigger_angle.set=pid_trigger_angle.get;}//��������ͣ     
		shoot.shoot_frequency=0;
	}	

}

int residual_heat;
void heat0_limit(void)           //��������
{  //���ڷ��͵�����Ϊ50hz������ÿ�ν���ò�����Ҫ5��ѭ������˼���ʱ ��ʽΪ����ȴ����-����ֵ+0.1*��ȴֵ
  shoot.limit_heart0 = judge_rece_mesg.game_robot_state.shooter_barrel_heat_limit-judge_rece_mesg.power_heat_data.shooter_id1_17mm_cooling_heat+0.002*judge_rece_mesg.game_robot_state.shooter_barrel_cooling_value;
	if(shoot.limit_heart0>residual_heat)
    {
    shoot.ctrl_mode=1;
    }
  else
    {
     shoot.ctrl_mode=0;
    }
}


//���� trigger poke
int buff_reversal_count;
int buff_time;
u8 buff_check_flag;
int single_shoot_cnt;
int press_l_cnt;

void shoot_bullet_handle(void)
{	
	static uint32_t start_shooting_count = 0;//��ת��ʱ
	static uint32_t start_reversal_count1 = 0;//��ת��ʱ
	static uint8_t lock_rotor1 = 0;//��ת��־λ
	static u8  press_l_flag;
	shoot.single_angle=45;

	heat_shoot_frequency_limit();//������Ƶ����

	
	if(gimbal_data.ctrl_mode!=GIMBAL_AUTO_SMALL_BUFF&&
		 gimbal_data.ctrl_mode!=GIMBAL_AUTO_BIG_BUFF)//����ģʽ
//        if(0)
	  {
          //��������
		  if(shoot.will_time_shoot>0&&
				 shoot.fric_wheel_run==1&&
							 shoot.poke_run==1&&				
						  shoot.ctrl_mode==1)
		{	
/**/
			start_shooting_count++;
			if((start_shooting_count >= 200)&&(abs(general_poke.poke.filter_rate) < 3))
			{
				lock_rotor1 = 1;
				start_shooting_count = 0;
			}
			if(lock_rotor1 == 1)
			{
				start_reversal_count1++;
			if(start_reversal_count1 > 100)
			{
				lock_rotor1 = 0;
				start_reversal_count1 = 0;
			}
			shoot.poke_pid.angle_ref=shoot.poke_pid.angle_fdb-shoot.shoot_frequency*45*36/500;		//һ��shoot_frequency����һ������ת45�㣬���ٱ���1��36��ÿ������ִ��һ�ιʳ���500��	
			shoot.poke_pid.angle_fdb=general_poke.poke.ecd_angle;
			shoot.poke_pid.speed_fdb=general_poke.poke.filter_rate;
			shoot.poke_current=pid_double_loop_cal(&pid_trigger_angle,&pid_trigger_speed,
                                                  shoot.poke_pid.angle_ref,
                                                  shoot.poke_pid.angle_fdb,
                                                 &shoot.poke_pid.speed_ref,
                                                    shoot.poke_pid.speed_fdb,0);
			}
			if((shoot.fric_wheel_run)&&(lock_rotor1 == 0))
			{	
/**/
			shoot.poke_pid.angle_ref=shoot.poke_pid.angle_fdb+shoot.shoot_frequency*45*36/500;		//һ��shoot_frequency����һ������ת45�㣬���ٱ���1��36��ÿ������ִ��һ�ιʳ���500��	
			shoot.poke_pid.angle_fdb=general_poke.poke.ecd_angle;
			shoot.poke_pid.speed_fdb=general_poke.poke.filter_rate;
			shoot.poke_current=pid_double_loop_cal(&pid_trigger_angle,&pid_trigger_speed,
                                                  shoot.poke_pid.angle_ref,
                                                  shoot.poke_pid.angle_fdb,
                                                 &shoot.poke_pid.speed_ref,
                                                    shoot.poke_pid.speed_fdb,0);
			}
		}
		else
		{
		pid_trigger_angle.set = pid_trigger_angle.get;//���ֱ���ͣ
		shoot.poke_current=0;
		start_shooting_count = 0;//������ת��ʱ
    start_reversal_count1 = 0;//���㷴ת��ʱ
            
		}

            buff_time = 0;
            single_shoot_cnt = 0;
}
	else//�������ģʽ
	{
        if(buff_time==0)
        {
            buff_check_flag = 1;
            shoot.poke_pid.angle_fdb=general_poke.poke.ecd_angle/36.109;
            shoot.poke_pid.angle_ref = shoot.poke_pid.angle_fdb;
        }
        buff_time++;
		if(shoot.fric_wheel_run==1)
		{
            if(RC_CtrlData.mouse.press_l==1)
            {
                press_l_cnt++;
            }
            else
            {
                press_l_cnt=0;
            }
            if(press_l_cnt>300)
            {
                if(time_tick%800==0)
                {
                    press_l_flag=0;
                }
            }
			if(RC_CtrlData.mouse.press_l==1||RC_CtrlData.RemoteSwitch.trigger==1)
			 {
                 if(press_l_flag==0)
				{
				press_l_flag=1;	
				shoot.poke_run=1;	
				}
				else
				{
				shoot.poke_run=0;	
				}
			}
		else
				{press_l_flag=0;}

         if(buff_check_flag)
         {
             buff_reversal_count++;
             if(buff_reversal_count<400)
             {
                 shoot.poke_pid.angle_ref-=shoot.single_angle;
                 
             }else
             {
                buff_reversal_count = 0;
                shoot.poke_pid.angle_ref = shoot.poke_pid.angle_fdb;
                 buff_check_flag = 0;
                 single_shoot_cnt++;
             }
         }else
         {
             if(shoot.poke_run==1)
             {
				shoot.poke_pid.angle_ref+=shoot.single_angle;
                 single_shoot_cnt++;
             }
             
         }		
			shoot.poke_pid.angle_fdb=general_poke.poke.ecd_angle/36.109f;
			shoot.poke_pid.speed_fdb=general_poke.poke.rate_rpm;
			shoot.poke_current=pid_double_loop_cal(&pid_trigger_angle_buf,&pid_trigger_speed_buf,
													 shoot.poke_pid.angle_ref,
													 shoot.poke_pid.angle_fdb,
													&shoot.poke_pid.speed_ref,
													 shoot.poke_pid.speed_fdb,0);
       
		}
		else
		{shoot.poke_pid.angle_ref=shoot.poke_pid.angle_fdb;
		shoot.poke_current=0;}
	}
}



//Ħ���� friction
void shoot_friction_handle()
{  
	if(shoot.fric_wheel_run==1)
	{
		pid_rotate[0].set= shoot.friction_pid.speed_ref[0] - Shooter_Bullet_Speed_Self_Adaptation(shoot.Bullet_Speed_Kalman.X_hat) ;
        pid_rotate[1].set= -shoot.friction_pid.speed_ref[0] + Shooter_Bullet_Speed_Self_Adaptation(shoot.Bullet_Speed_Kalman.X_hat) ;	
	}
	else
	{
    pid_rotate[0].set=0;
    pid_rotate[1].set=0;
  }
	pid_rotate[0].get = general_friction.left_motor.filter_rate;
  pid_rotate[1].get = general_friction.right_motor.filter_rate;

  shoot.fric_current[0]=pid_calc(& pid_rotate[0],pid_rotate[0].get, pid_rotate[0].set);
  shoot.fric_current[1]=pid_calc(& pid_rotate[1],pid_rotate[1].get, pid_rotate[1].set);
}	




/**
************************************************************************************************************************
* @Name     : shoot_state_mode_switch
* @brief    : ң����/���� ����״̬����
* @retval   : 
* @Note     : 
************************************************************************************************************************
**/
int press_C_cnt;
void shoot_state_mode_switch()
{
	 /****************************�������״̬����**********************************************/		
		switch(RC_CtrlData.inputmode)
			{
					case REMOTE_INPUT:
				{
					if(RC_CtrlData.RemoteSwitch.s3to1)
					{
						shoot.fric_wheel_run=1;
						if(gimbal_data .auto_aim_rotate_flag==1)
						{ LASER_OFF();}
						else
						{ LASER_ON();}
					}
					 else
					{	
						shoot.fric_wheel_run=0;
						LASER_OFF();
					}
					
					if(gimbal_data.auto_aim_rotate_flag==1)
					{if(RC_CtrlData.rc.ch4>1500)
					{
						if(New_Auto_Aim.enable_shoot==1) 
						{shoot.poke_run=1;}
						else
						{shoot.poke_run=0;}
					}
					else
						{shoot.poke_run=0;}

					}
					else
					{if(RC_CtrlData.rc.ch4>1500)
						{shoot.poke_run=1;}
						else
							{shoot.poke_run=0;}
					}
			}break;
					case KEY_MOUSE_INPUT:
				{
					if(RC_CtrlData.mouse.press_r==1)
					{
						if(RC_CtrlData.mouse.press_l==1&&New_Auto_Aim.enable_shoot==1)
						{shoot.poke_run=1;}
					else
					{shoot.poke_run=0;}
					}else
					{
						if(RC_CtrlData.mouse.press_l==1)
						{shoot.poke_run=1;}
					  else
						{	shoot.poke_run=0;}
					}
					
                   if(RC_CtrlData.Key_Flag.Key_C_Flag)
                   {
                       press_C_cnt++;
                       if(press_C_cnt < 1000)
                       {
                           shoot.fric_wheel_run=1;
												  if(gimbal_data .auto_aim_rotate_flag==1)
                          { LASER_OFF();}
													else
													{ LASER_ON();}
                       }else
                       {
                           shoot.fric_wheel_run=0;
                           LASER_OFF();
                       }
                   }else
                   {
                      press_C_cnt = 0;
                   }
					 
					if(RC_CtrlData.Key_Flag.Key_Q_TFlag)
						 {shoot.bulletspead_level=1;}
					else
						 {shoot.bulletspead_level=0;}
				 }break;
				case STOP:
				{
					shoot.fric_wheel_run=0;
					shoot.poke_run=0;
          buff_time = 0;
          single_shoot_cnt = 0;
				}break;

					default:
					break;		
			}
}

/**
************************************************************************************************************************
* @Name     : shoot_task
* @brief    : ���亯��������
* @retval   : 
* @Note     : 
************************************************************************************************************************
**/
void shoot_task()
{

	shoot_state_mode_switch();

    heat0_limit();
	shoot_bullet_handle();
	shoot_friction_handle();


}

/**
************************************************************************************************************************
* @Name     : auto_bullet_speed
* @brief    : ����Ӧ����
* @retval   : 
* @Note     : 
************************************************************************************************************************
**/


float First_Order_Kalman_Filter_Cal
        (First_Order_Kalman_Filter_t *_First_Order_Kalman_Filter
        ,float _Z/*����ֵ*/)
{
        //������һ�ε�ֵ
        _First_Order_Kalman_Filter->Error_Est_Last=_First_Order_Kalman_Filter->Error_Est;
        _First_Order_Kalman_Filter->X_hat_Last=_First_Order_Kalman_Filter->X_hat;
        //������һ�ε�ֵ
        _First_Order_Kalman_Filter->Error_Est_Last=_First_Order_Kalman_Filter->Error_Est;
        _First_Order_Kalman_Filter->X_hat_Last=_First_Order_Kalman_Filter->X_hat;
        //Step1:Kalman_Gain����
        _First_Order_Kalman_Filter->Kalman_Gain=
        _First_Order_Kalman_Filter->Error_Est_Last
        /(_First_Order_Kalman_Filter->Error_Est_Last+_First_Order_Kalman_Filter->Error_Mea);
        //Step2:����Ԥ��ֵ
        _First_Order_Kalman_Filter->X_hat=
        _First_Order_Kalman_Filter->X_hat_Last
        +_First_Order_Kalman_Filter->Kalman_Gain*(_Z-_First_Order_Kalman_Filter->X_hat_Last);
        //Step3:Ԥ��������
        _First_Order_Kalman_Filter->Error_Est=(1-_First_Order_Kalman_Filter->Kalman_Gain)
        *_First_Order_Kalman_Filter->Error_Est_Last;
        //����Ԥ��ֵ
        return _First_Order_Kalman_Filter->X_hat;
}

