/* Includes ------------------------------------------------------------------*/
#include "CHASSIS_TASK.h"
/*----------------------------------------------------------------------------*/
/**
  ******************************************************************************
  * @file    chassis.c
  * @author  TC
  * @version V1.2.0
  * @date    2023.6.10
  * @brief   此文件编写舵轮底盘功能及功率限制
						 均在头文件通过宏定义更改
						 《主调用函数下拉至底部》
@verbatim
 ===============================================================================
 **/
/* Variables_definination-----------------------------------------------------------------------------------------------*/
chassis_t 		 		 chassis        = CHASSIS_DEFAULT;
Chassis_angle_t  Chassis_angle = CHASSIS_ANGLE_DEFAULT;

float I6020[4],sdpower,shpower,dpower[4],hpower[4],all_power1[4],all_power2[4],
			all_power,a6020[4],b6020[4],m6020,n6020,l6020,i6020,kall6020[4];
float k6020=-0.000075,k2_6020=1.278e-7,k1_6020=6.157e-6,k0_6020=1.054;
/*----------------------------------------------------------------------------------------------------------------------*/
float test_in,test_out;

void chassis_param_init(chassis_t *chassis)//底盘参数初始化
{

//	memset(&chassis, 0, sizeof(chassis_t));
	chassis->ctrl_mode      = CHASSIS_STOP;
	chassis->last_ctrl_mode = CHASSIS_RELAX;

	PID_struct_init(&pid_cha_6020_angle[0], POSITION_PID, 8000, 10, 7,0.1f,5);//24, 0.2f,20);
	PID_struct_init(&pid_cha_6020_speed[0], POSITION_PID, 14000, 500, 200,0.1f,10);//38,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[1], POSITION_PID, 8000, 10, 7,0.1f,5);//25, 0.2f,15);
	PID_struct_init(&pid_cha_6020_speed[1], POSITION_PID, 14000, 500, 200,0.1f,4);//39,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[2], POSITION_PID, 8000, 10, 7,0.1f,4);//20, 0.2f,20);
	PID_struct_init(&pid_cha_6020_speed[2], POSITION_PID, 14000, 500, 200,0.5,8);//40,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[3], POSITION_PID, 8000, 10, 7,0.1f,4);//23, 0.2f,15);
	PID_struct_init(&pid_cha_6020_speed[3], POSITION_PID, 14000, 500, 200,0.1f,10);//42,0.5f,20);

	for (int k = 0; k < 4; k++)
    {
		PID_struct_init(&pid_cha_3508_speed[k], POSITION_PID,15000, 6000,60,0.0f, 0); //24 0.3 10    38.0f,3.0f, 40
    }
    PID_struct_init(&pid_chassis_angle, POSITION_PID, 4000, 10, 600,0,50);
	
	ChassisSpeedRamp0.Init(&ChassisSpeedRamp0,0.1,Chassis_angle.handle_speed_lim[0],Chassis_angle.handle_speed_lim[0]);
	ChassisSpeedRamp1.Init(&ChassisSpeedRamp1,0.1,Chassis_angle.handle_speed_lim[1],Chassis_angle.handle_speed_lim[1]);
	ChassisSpeedRamp2.Init(&ChassisSpeedRamp2,0.001,Chassis_angle.handle_speed_lim[2],Chassis_angle.handle_speed_lim[2]);
	ChassisSpeedRamp3.Init(&ChassisSpeedRamp3,0.001,Chassis_angle.handle_speed_lim[3],Chassis_angle.handle_speed_lim[3]);
	Test_Ramp.Init(&Test_Ramp,0.5,test_in,test_in);

}
//主要功率控制部分
#if POWER_LIMIT_HANDLE 
/**
************************************************************************************************************************
* @Name     : power_limit_handle
* @brief    : 功率控制的主函数
* @param		: None
* @retval   : void
* @Note     : 
************************************************************************************************************************
**///舵轮
void power_limit_handle(chassis_t *chassis)
{
		chassis->chassis_power.Cap_V=usart_capacitance_message.cap_voltage_filte;
		VAL_LIMIT(chassis->chassis_power.Cap_V,0,25);
	
	
//	if(chassis->chassis_power.Cap_V>15)
//	 {		
//		chassis->chassis_power.Max_Chassis_Power=get_max_power2(chassis->chassis_power.Cap_V);//(float)((usart_capacitance_message.cap_voltage_filte)-chassis.chassis_power.Max_Power_6020)
//		chassis->chassis_power.power_limit_rate=get_the_limite_rate(420);
//	 }
//	else
//	 {
//		chassis->chassis_power.Max_Chassis_Power=get_max_power2(chassis->chassis_power.Cap_V);//(float)((usart_capacitance_message.cap_voltage_filte)-chassis.chassis_power.Max_Power_6020)
//		get_6020power();
//		chassis->chassis_power.power_limit_rate=get_the_limite_rate(chassis->chassis_power.Max_Chassis_Power-chassis->chassis_power.Max_Power_6020);
//	 }
	 
		chassis->chassis_power.Max_Chassis_Power=get_max_power2(chassis->chassis_power.Cap_V);//(float)((usart_capacitance_message.cap_voltage_filte)-chassis.chassis_power.Max_Power_6020)
		get_6020power();
		chassis->chassis_power.power_limit_rate=get_the_V_limite_rate(50);
	 

  	VAL_LIMIT(chassis->chassis_power.power_limit_rate,0,1);		 
		
}
/**
************************************************************************************************************************
* @Name     : get_max_power1  get_max_power2
* @brief    : 用于获取最大功率控制
* @param		: 超级电容电压值
* @retval   : 最大功率
* @Note     : get_max_power1/get_max_power2舵步
************************************************************************************************************************
**///步兵功率限制

float max_power=0;

float get_max_power2(float voltage)
{

	{max_power=voltage*16-25;}

	VAL_LIMIT(max_power,0,voltage*16);
  return max_power;
}
/**
************************************************************************************************************************
* @Name     : get_the_limite_rate
* @brief    : 根据给定最大功率求出最优功率限制系数
* @param		: max_power
* @retval   : 
* @Note     :
************************************************************************************************************************
**/

static float get_the_V_limite_rate(float max_power)
{
	float a[4];
	for(int i=0; i<4; i++)
	{	a[i]=(float)chassis.cha_pid_3508.speed_ref[i] * (pid_cha_3508_speed[i].p+pid_cha_3508_speed[i].d);}
	
	float b[4];
	for(int i=0; i<4; i++)
   { b[i]=-pid_cha_3508_speed[i].p*(float)chassis.cha_pid_3508.speed_fdb[i]+pid_cha_3508_speed[i].iout \
         -pid_cha_3508_speed[i].d*(float)chassis.cha_pid_3508.speed_fdb[i]-pid_cha_3508_speed[i].d*pid_cha_3508_speed[i].err[LAST];}
		
				 
// Max_power=heat_power+drive_power
//	i_n=a[n]*k+b[n]	带入
//Max_Power=m*k^2+n*k+o
//0=m*k^2+n*k+l(l=o-Max_Power)
	float m=(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]+a[3]*a[3])*FACTOR_2;

	float n=2*FACTOR_2*(a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3]) + \
		  FACTOR_1*(a[0] + a[1] + a[2] + a[3]) + \
		  I_TIMES_V_TO_WATT*(a[0]*(float)chassis.cha_pid_3508.speed_fdb[0] + \
							 a[1]*(float)chassis.cha_pid_3508.speed_fdb[1] + \
							 a[2]*(float)chassis.cha_pid_3508.speed_fdb[2] + \
							 a[3]*(float)chassis.cha_pid_3508.speed_fdb[3]);

	float l=(b[0]*b[0] + b[1]*b[1] + b[2]*b[2] + b[3]*b[3])*FACTOR_2 + \
		  (b[0] + b[1] + b[2] + b[3])*FACTOR_1 + \
		  I_TIMES_V_TO_WATT*(b[0]*(float)chassis.cha_pid_3508.speed_fdb[0] + \
							 b[1]*(float)chassis.cha_pid_3508.speed_fdb[1] + \
							 b[2]*(float)chassis.cha_pid_3508.speed_fdb[2] + \
							 b[3]*(float)chassis.cha_pid_3508.speed_fdb[3]) + \
		  4*FACTOR_0 - \
		  max_power;
	return (-n+(float)sqrt((double)(n*n-4*m*l)+1.0f))/(2*m);
}

static float get_T_limit_rate(float max_power)
{
    float a = 0;
    float b = 0;
    float c = 0;
    
    for(int i = 0;i<4;i++)
    {
        a+=FACTOR_2*(float)(pid_cha_3508_speed[i].out*pid_cha_3508_speed[i].out);
        b+=I_TIMES_V_TO_WATT*(float)chassis.cha_pid_3508.speed_fdb[i]*pid_cha_3508_speed[i].out + \
            FACTOR_1*(float)pid_cha_3508_speed[i].out;
    }
    c = 4*FACTOR_0 - max_power;
    
    return (-b+(float)sqrt((double)(b*b-4*a*c)+1.0f))/(2*a);
}
/**
************************************************************************************************************************
* @Name     : get_6020power
* @brief    : 根据给定功率求6020功率限制系数 
* @param		: 专用于舵轮
* @retval   : kall6020[i]
* @Note     : 
************************************************************************************************************************
**/
float get_6020power()
{
	for(int i=0;i<4;i++)
	{
		I6020[i] = pid_calc(&pid_cha_6020_speed[i], chassis.cha_pid_6020.speed_fdb[i], chassis.cha_pid_6020.speed_ref[i]);
		VAL_LIMIT(I6020[i],-10000,10000);	// 6020电压最大发送值为25000
		dpower[i] = I6020[i]*pid_cha_6020_speed[i].get*k6020;
		hpower[i] = k2_6020*I6020[i]*I6020[i]+k1_6020*I6020[i]+k0_6020;
		all_power1[i] = dpower[i]+hpower[i];
	}
	all_power = all_power1[0]+all_power1[1]+all_power1[2]+all_power1[3];
	VAL_LIMIT(all_power,0,1000);
	chassis.chassis_power.Max_Power_6020=all_power;
	VAL_LIMIT(chassis.chassis_power.Max_Power_6020,0,chassis.chassis_power.Max_Steering_Power);
	chassis.chassis_power.Max_Power_3508 = chassis.chassis_power.Max_Chassis_Power-chassis.chassis_power.Max_Power_6020;

	float a[4],b[4],c[4];
	if(all_power>chassis.chassis_power.Max_Steering_Power)
	{	
		for(int i = 0; i < 4; i++)
		{
			all_power2[i]=all_power1[i]*chassis.chassis_power.Max_Steering_Power/all_power;
			if(all_power1[i]>k0_6020&&all_power1[i]>all_power2[i])//或许all_power2>k0_6020更好
			{
				a[i]=k2_6020;
				b[i]=k1_6020+k6020*pid_cha_6020_speed[i].get;
				c[i]=k0_6020-all_power2[i];
				kall6020[i]=(-b[i]+(double)sqrt(b[i]*b[i]-4*a[i]*c[i]))/(2*a[i])/I6020[i];
			}
			else
			{	kall6020[i]=1;	}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		kall6020[i]=1;
	}
	for (int i = 0; i < 4; i++)
		VAL_LIMIT(kall6020[i],0,1);
}

float get_6020_T_limit_rate(float max_power)
{
    
}
#endif
/**
************************************************************************************************************************
* @Name     : convert_ecd_angle_to_0_2pi
* @brief    : 将电机编码器的机械角度值（范围正负无穷大）解算为范围在0~2pi的角度值      
* @param		: ecd_angle 电机编码器的机械角度值  类型  double
* @param		: _0_2pi_angle 范围在0~2pi的角度值  类型  float
* @retval   : _0_2pi_angle 范围在0~2pi的角度值  类型  float
* @Note     : 
************************************************************************************************************************
**/
void convert_ecd_angle_to_0_2pi(Chassis_angle_t *chassis_angle,double ecd_angle)
{ 
	/*机械角度值 赋值 范围正负无穷大*/
	chassis_angle->yaw_encoder_ecd_angle=ecd_angle;
	/*考虑Yaw轴电机极性 将其机械角度值转换到0-2PI范围内*/
	chassis_angle->yaw_angle_0_2pi=fmod(ecd_angle*ANGLE_TO_RAD*YAW_POLARITY,2*PI);	
	
	if(chassis_angle->yaw_angle_0_2pi<0){chassis_angle->yaw_angle_0_2pi+=2*PI;}
		
	if(chassis_angle->yaw_angle_0_2pi>=PI)/*将0-2PI转换到-PI-PI范围内*/
	{chassis_angle->yaw_angle__pi_pi=chassis_angle->yaw_angle_0_2pi-(2*PI);}
	else
	{chassis_angle->yaw_angle__pi_pi=chassis_angle->yaw_angle_0_2pi;}
}

/**
************************************************************************************************************************
* @Name     : 
* @brief    : chassis_mode_select函数下的各实现函数
* @retval   : 
* @Note     : 
*							chassis_stop_handle          底盘关控				
*							follow_gimbal_handle         底盘跟随云台模式控制 主要是vx vy vw的获取
*							rotate_follow_gimbal_handle	 小陀螺模式控制
************************************************************************************************************************
**/
void chassis_stop_handle(chassis_t *chassis)
{
  chassis->vy = 0;
  chassis->vx = 0;
	chassis->vw = 0;
  chassis->get_speedw = 0;//舵轮使用
}

void follow_gimbal_handle(chassis_t *chassis)
{
	chassis->vy = uart_cha_data.y;
  chassis->vx = uart_cha_data.x;
//	chassis.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0);   // 跟随正方向
		/* 矢量 */
	chassis->get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, 0);
}

void rotate_follow_gimbal_handle(chassis_t *chassis)
{
					if(uart_cha_data.speed_mode==HIGH_SPEED_MODE&&chassis->vx==0&&chassis->vy==0)
					{
						chassis->vw=(2.2-0.4*Sinusoidal_Waveform_Generator(0.002/*该函数执行周期*/,0.33/*生成正弦波频率*/))*350;
					}
					else
					{
						if(judge_rece_mesg.game_robot_state.robot_level==0)
						chassis->vw=700;		//200
						if(judge_rece_mesg.game_robot_state.robot_level==1)
						chassis->vw=500;		//200
						if(judge_rece_mesg.game_robot_state.robot_level==2)
						chassis->vw=500;
						if(judge_rece_mesg.game_robot_state.robot_level==3)
						chassis->vw=500;
						if(judge_rece_mesg.game_robot_state.robot_level==4)
						chassis->vw=550;
						if(judge_rece_mesg.game_robot_state.robot_level==5)
						chassis->vw=550;
						if(judge_rece_mesg.game_robot_state.robot_level==6)
						chassis->vw=600;
						if(judge_rece_mesg.game_robot_state.robot_level==7)
						chassis->vw=600;
						if(judge_rece_mesg.game_robot_state.robot_level==8)
						chassis->vw=650;
						if(judge_rece_mesg.game_robot_state.robot_level==9)
						chassis->vw=700;
						if(judge_rece_mesg.game_robot_state.robot_level==10)
						chassis->vw=700;
					}
	chassis->vy = uart_cha_data.y;
	chassis->vx = uart_cha_data.x;
		
	if(chassis->last_ctrl_mode!=CHASSIS_ROTATE)
	{
		if(chassis->chassis_flag.get_speedw_flag==0)
		{chassis->chassis_flag.get_speedw_flag=1;}
		else if(chassis->chassis_flag.get_speedw_flag==1)
		{chassis->chassis_flag.get_speedw_flag=0;}
	}
	if(chassis->chassis_flag.get_speedw_flag==0)
	{chassis->get_speedw = -chassis->vw;}
	else if(chassis->chassis_flag.get_speedw_flag==1)
	{chassis->get_speedw = chassis->vw;}
	

}

/**
************************************************************************************************************************
* @Name     :start_angle_handle
* @brief    :解算出初始角，偏航角，驱动轮速度，以及限制级驱动轮速度
* @param		:None 
* @retval   :void
* @Note     :加入功率限制标志位的驱动轮速度给定 是实际发送的值	
							10.3 考虑transition3为nan的情况
************************************************************************************************************************
**/

/**
 * @brief  基于 直接 矢量合成 的舵轮解算
 * */
void steering_wheel_calc(Chassis_angle_t *chassis_angle)
{
	float vx = 0;
	float vy = 0;
	float relative_angle = -chassis_angle->yaw_angle__pi_pi;
	
	/* 小陀螺角度补偿 */
	if(chassis.ctrl_mode==CHASSIS_ROTATE)
	{
		if(chassis.chassis_flag.get_speedw_flag == 1)
			relative_angle += 11.0f *ANGLE_TO_RAD;		//13 6.5
		else if(chassis.chassis_flag.get_speedw_flag == 0)
			relative_angle -= 12.5f *ANGLE_TO_RAD;		//13 6.5
	}
	
  chassis.vx_ramp=ChassisSpeedRamp0.Calc(&ChassisSpeedRamp0,chassis.vx-chassis.vx_ramp,chassis.vx,chassis.vx);
	chassis.vy_ramp=ChassisSpeedRamp1.Calc(&ChassisSpeedRamp1,chassis.vy-chassis.vy_ramp,chassis.vy,chassis.vy);
	
	for(uint8_t i = 0; i < 4; i++)
	{
//		vx = chassis.vx*cosf(relative_angle) - chassis.vy*sinf(relative_angle) + chassis.get_speedw*cosf(-1.0f*PI/4.0f + i*PI/2.0f);
//		vy = chassis.vx*sinf(relative_angle) + chassis.vy*cosf(relative_angle) + chassis.get_speedw*sinf(-1.0f*PI/4.0f + i*PI/2.0f);
		vx = chassis.vx*cosf(relative_angle) - chassis.vy*sinf(relative_angle) + chassis.get_speedw*cosf(-1.0f*PI/4.0f + i*PI/2.0f);
		vy = chassis.vx*sinf(relative_angle) + chassis.vy*cosf(relative_angle) + chassis.get_speedw*sinf(-1.0f*PI/4.0f + i*PI/2.0f);
		chassis_angle->handle_speed_lim[i] = sqrt(vx*vx + vy*vy);
		chassis_angle->deviation_angle[i]  = (float)atan2(vy, vx)*RAD_TO_ANGLE;
	}
	
}

/**
************************************************************************************************************************
* @Name     : steer_optimizing
* @brief    : 航向轴电机优劣弧优化
* @param    : none
* @retval   : void 
* @Note     : 转劣弧的处理
************************************************************************************************************************
**///
void steer_optimizing(void)
{
	int static K_A[4]={0};
	int static K_S[4]={1,1,1,1};
	for(int i=0;i<4;i++)
	{
		chassis.cha_pid_6020.angle_fdb[i] =steering_wheel_chassis.Heading_Encoder[i].ecd_angle*STEERING_POLARITY;
		chassis.cha_pid_6020.angle_ref[i] =Chassis_angle.deviation_angle[i]+K_A[i]*180;	
		Chassis_angle.handle_speed_lim[i] =K_S[i]*Chassis_angle.handle_speed_lim[i];
	}
	Chassis_angle.handle_speed_lim[0] = RIGHT_FRONT_REVERSE*Chassis_angle.handle_speed_lim[0];
	Chassis_angle.handle_speed_lim[1] = LEFT_FRONT_REVERSE*Chassis_angle.handle_speed_lim[1];
	Chassis_angle.handle_speed_lim[2] = LEFT_BEHIND_REVERSE*Chassis_angle.handle_speed_lim[2];
	Chassis_angle.handle_speed_lim[3] = RIGHT_BEHIND_REVERSE*Chassis_angle.handle_speed_lim[3];
	
	for(int i=0;i<4;i++)
	{
		if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])>90&&(chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])<270)
		{	  
			K_A[i]-=1;
			K_S[i]=-K_S[i];
		}
		else if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])>270)
		{K_A[i]-=2;}
		else if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])<-90&&(chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])>-270)
		{
			K_A[i]+=1;
			K_S[i]=-K_S[i];
		}
		else if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])<-270)
		{K_A[i]+=2;}
 	}
}

/**
************************************************************************************************************************
* @Name     : start_chassis_6020
* @brief    : 航向轴电机pid速度环/角度环 反馈值的获取，给定值的设置
* @param    : none
* @retval   : void 
* @Note     : 
************************************************************************************************************************
**/
void start_chassis_6020()
{		

	for(int i=0;i<4;i++)
		pid_calc(&pid_cha_6020_angle[i],chassis.cha_pid_6020.angle_fdb[i],chassis .cha_pid_6020.angle_ref[i]);

	if(chassis .ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE)
	{
		for(int j=0;j<4;j++)
		{chassis.cha_pid_6020.speed_fdb[j]=steering_wheel_chassis.Heading_Encoder[j].filter_rate * STEERING_POLARITY;
		
		chassis.cha_pid_6020.speed_ref[j]=pid_cha_6020_angle[j].out;
		
		#if POWER_LIMIT_HANDLE
		pid_calc(&pid_cha_6020_speed[j],chassis.cha_pid_6020.speed_fdb[j], kall6020[j] * chassis.cha_pid_6020.speed_ref[j]);//
		#else
		pid_calc(&pid_cha_6020_speed[j],chassis.cha_pid_6020.speed_fdb[j],chassis.cha_pid_6020.speed_ref[j]);
		#endif
		}
	}
		/* 去除急刹 */		
	  static u32 DWT_stop_cnt=0;
		if(chassis.vx == 0 && chassis.vy == 0  && fabs(chassis.get_speedw) < 50)
		{
			if(DWT_stop_cnt==0){DWT_stop_cnt=DWT->CYCCNT;}
			if((u32)(DWT->CYCCNT/168000)-DWT_stop_cnt<1000)
			{	/*1000ms后自锁*/
				for(uint8_t i = 0; i < 4; i++)
				{
					pid_cha_6020_speed[i].out = 0;
				}
			}
//			else if((u32)(DWT->CYCCNT/168000)-DWT_stop_cnt>5000)
//			{
//				for(uint8_t i = 0; i < 4; i++)
//				{
//					pid_cha_6020_speed[i].max_out=3000;
//				}
//			}
		}
		else if(!(chassis.vx == 0 && chassis.vy == 0 ))
		{
			DWT_stop_cnt = 0;
//			if(fabs(chassis.get_speedw) > 50)
//			{
//				for(uint8_t i = 0; i < 4; i++)
//				{
//					pid_cha_6020_speed[i].max_out=15000;
//				}
//			}
		}
						
		for (int i = 0; i < 4; i++)
		{   
			if((chassis.ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE))
			{chassis.voltage[i]=1.0f*(int16_t)pid_cha_6020_speed[i].out*STEERING_POLARITY;	}
			else
			{chassis.voltage[i]=0;}
		}	
}
/**
************************************************************************************************************************
* @Name     :start_chassis_3508
* @brief    :驱动轮电机速度环反馈获取和给定设置；驱动轮和航向轴电机pid最终给定计算处理
* @param    :None
* @retval   :void
* @Note     :功率控制切换限制
************************************************************************************************************************
**/
void start_chassis_3508(void)
{		
	
chassis.chassis_flag.two_wheel_mode=0;
	for (int i = 0; i < 4; i++)
	{ 
		chassis.cha_pid_3508.speed_fdb[i]=steering_wheel_chassis.Driving_Encoder[i].filter_rate;
		chassis.cha_pid_3508.speed_ref[i]=Chassis_angle.handle_speed_lim[i];
		if(Chassis_angle.Remote_speed==0&&fabs(chassis.get_speedw)<5)
		{chassis.cha_pid_3508.speed_ref[i]=0;}
		
			
			
		
	#if POWER_LIMIT_HANDLE
		chassis.Power_Limit(&chassis);
	#endif
		
		#if POWER_LIMIT_HANDLE
		pid_calc(&pid_cha_3508_speed[i],chassis.cha_pid_3508.speed_fdb[i], /*chassis.chassis_power.power_limit_rate * */chassis.cha_pid_3508.speed_ref[i]); 
		#else
		pid_calc(&pid_cha_3508_speed[i],chassis.cha_pid_3508.speed_fdb[i],chassis.cha_pid_3508.speed_ref[i]);
		#endif
	}
    
	chassis.chassis_power.power_T_limlit_rate = get_T_limit_rate(50);
    VAL_LIMIT(chassis.chassis_power.power_T_limlit_rate,0,1);
			/* 死区处理 */
//		for(int i = 0; i < 4; i++)
//		{   
			if((chassis.ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE))				
			{
                
                chassis.current[0] =chassis.chassis_power.power_T_limlit_rate*(int16_t)pid_cha_3508_speed[0].out*1.0;
                chassis.current[1] =chassis.chassis_power.power_T_limlit_rate*(int16_t)pid_cha_3508_speed[1].out*1.0;
                chassis.current[2] =chassis.chassis_power.power_T_limlit_rate*(int16_t)pid_cha_3508_speed[2].out*1.0;	
                chassis.current[3] =chassis.chassis_power.power_T_limlit_rate*(int16_t)pid_cha_3508_speed[3].out*1.0;	
            }	
			else
			{
                chassis.current[0]=0;
                chassis.current[1]=0;
                chassis.current[2]=0;
                chassis.current[3]=0;
            }
			
			
//		}			
}
/**
************************************************************************************************************************
* @Name     : get_remote_set
* @brief    : 解算遥控器速度方向和大小
* @param    : None
* @retval   : void
* @Note     : 10.3 优化了算法更精确更高刷新率
************************************************************************************************************************
**/
void get_remote_set(Chassis_angle_t *chassis_angle)
{	
	  float temp_angle=0;
		chassis_angle->vx = uart_cha_data.x;
		chassis_angle->vy = uart_cha_data.y;
		chassis_angle->Remote_speed = sqrt((chassis_angle->vx*chassis_angle->vx)+(chassis_angle->vy*chassis_angle->vy));
		if(chassis_angle->Remote_speed >= 50)
		{	
			temp_angle=atan2(chassis_angle->vy,chassis_angle->vx);
			chassis_angle->Remote_angle = fmod(2*PI+temp_angle,2*PI);
		}
		else
		{chassis_angle->Remote_speed = 0;}
}
/**
************************************************************************************************************************
* @Name     : chassis_mode_select
* @brief    : 将上位机发送的命令经选择后具体实行
* @param		: none
* @retval   : void
* @Note     : 
************************************************************************************************************************
**/
void chassis_mode_select(chassis_t *chassis)
{
	chassis->ctrl_mode=uart_cha_data.chassis_mode;
	switch (chassis->ctrl_mode)
    {
		case CHASSIS_STOP:					//底盘静止
		{chassis->STOP_(chassis);}
		break;
		case MANUAL_FOLLOW_GIMBAL:   //跟随云台模式
		{chassis->FOLLOW(chassis);}
		break;
		case CHASSIS_ROTATE:         //小陀螺
		{chassis->ROTATE(chassis);}
		break;
		default:
		{chassis->STOP_(chassis);}
		break;
    }
	chassis->last_ctrl_mode=uart_cha_data.chassis_mode;
}

/**
************************************************************************************************************************
* @Name     : Chassis_PID_handle
* @brief    : 
* @param		: none
* @retval   : void
* @Note     : 
************************************************************************************************************************
**/
void Chassis_PID_handle(chassis_t *chassis)
{

		/*优劣弧优化*/
	steer_optimizing();
		/* 6020 speed ref 计算 */
	start_chassis_6020();
	  /* 3508 speed ref 计算 */
	start_chassis_3508();
		/* 速度环pid计算 + 功率限制 */
}


void Motion_resolution(Chassis_angle_t *chassis_angle,double ecd_angle)
{
		Chassis_angle.Get_Yaw_Data(chassis_angle,ecd_angle);//获取yaw轴相关数据
		Chassis_angle.Get_RC_Data(chassis_angle);//获取遥控器数据
	  Chassis_angle.Get_Steer_Para(chassis_angle);//舵轮解算
}

/**
************************************************************************************************************************
* @Name     : chassis_task
* @brief    : 模式选择->运动解算->反馈控制
* @retval   : None
* @Note     : 
************************************************************************************************************************
**/
void chassis_task()
{	
	chassis.Mode_Select(&chassis);
	Chassis_angle.Motion_Soving(&Chassis_angle,uart_cha_data.yaw_Encoder_ecd_angle);
	chassis.PID_Calc(&chassis);
}