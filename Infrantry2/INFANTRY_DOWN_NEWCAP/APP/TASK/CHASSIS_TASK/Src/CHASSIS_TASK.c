/* Includes ------------------------------------------------------------------*/
#include "CHASSIS_TASK.h"
/*----------------------------------------------------------------------------*/
/**
  ******************************************************************************
  * @file    chassis.c
  * @author  TC
  * @version V1.1.0
  * @date    11-October-2023
  * @brief   ���ļ���д����ͨ�õ��̣���������/���� �Լ���Ӧ���ֹ�������
						 ����ͷ�ļ�ͨ���궨�����
						 �������ú����������ײ���
@verbatim
 ===============================================================================
 **/

/* Variables_definination-----------------------------------------------------------------------------------------------*/
Chassis_angle_t 	 Chassis_angle;
chassis_t 		 		 chassis;

float pid_chassis_angle_deathband = 15.0f*3.14/180;

float R,theta;
int   yaw_num_get,run_flag;
double yaw_ecd_angle;
int16_t vx,vy;
int getnumb[4];

float transition1[4];
float transition2[4];
float transition3[4];
float transition4[4];

float retransition1[4];
float retransition2[4];
float retransition3[4];
float retransition4[4];

int yaw_num_get;
float angle_flag[4];
float deviation_angle_flag[4];

float volatilAo;
u16 gyro_speed=400;
u8  get_speedw_flag;

u8      cap_flag;
int16_t cap_flag_time;
u16  Max_Power;
u16  Max_Power_3508;
u16  Max_Power_6020;
float power_limit_start_time;
float power_limit_start_flag;
/* �������Ʊ��� */
float power_limit_rate2;
float power_limit_rate1;	

float  max_chassis_power =35;
float  max_chassis_power1=0;
float I6020[4],sdpower,shpower,dpower[4],hpower[4],all_power1[4],all_power2[4],
	all_power,a6020[4],b6020[4],m6020,n6020,l6020,i6020,kall6020[4];

float k6020=-0.000075,k2_6020=1.278e-7,k1_6020=6.157e-6,k0_6020=1.054;
/*----------------------------------------------------------------------------------------------------------------------*/

void chassis_param_init()//���̲�����ʼ��
{
	memset(&chassis, 0, sizeof(chassis_t));
	chassis.ctrl_mode      = CHASSIS_STOP;
	chassis.last_ctrl_mode = CHASSIS_RELAX;

	chassis.position_ref = 0;

//    PID_struct_init(&pid_cha_6020_angle[0], POSITION_PID, 8000, 10, 7,0.1f,5);//24, 0.2f,20);
//  	PID_struct_init(&pid_cha_6020_speed[0], POSITION_PID, 15000, 500, 210,0.1f,10);//38,0.5f,20);
//	
//		PID_struct_init(&pid_cha_6020_angle[1], POSITION_PID, 8000, 10, 7,0.1f,5);//25, 0.2f,15);
//  	PID_struct_init(&pid_cha_6020_speed[1], POSITION_PID, 15000, 500, 180,0.1f,4);//39,0.5f,20);
//	
//	  PID_struct_init(&pid_cha_6020_angle[2], POSITION_PID, 8000, 10, 7,0.1f,4);//20, 0.2f,20);
//  	PID_struct_init(&pid_cha_6020_speed[2], POSITION_PID, 15000, 500, 200,0.5,8);//40,0.5f,20);
//		
//    PID_struct_init(&pid_cha_6020_angle[3], POSITION_PID, 8000, 10, 7,0.4f,4);//23, 0.2f,15);
//  	PID_struct_init(&pid_cha_6020_speed[3], POSITION_PID, 15000, 500, 200,0.1f,20);//42,0.5f,20);
    
//    20000, 3000, 150,1.0f,10);
//    20000, 0, 40,0.0f,0);
    
	PID_struct_init(&pid_cha_6020_angle[0], POSITION_PID, 8000, 10, 8,0.1f,5);//24, 0.2f,20);
	PID_struct_init(&pid_cha_6020_speed[0], POSITION_PID, 15000, 500, 200,0.1f,10);//38,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[1], POSITION_PID, 8000, 10, 8,0.1f,5);//25, 0.2f,15);
	PID_struct_init(&pid_cha_6020_speed[1], POSITION_PID, 15000, 500, 200,0.1f,4);//39,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[2], POSITION_PID, 8000, 10, 8,0.1f,4);//20, 0.2f,20);
	PID_struct_init(&pid_cha_6020_speed[2], POSITION_PID, 15000, 500, 200,0.5,8);//40,0.5f,20);

	PID_struct_init(&pid_cha_6020_angle[3], POSITION_PID, 8000, 10, 8,0.1f,4);//23, 0.2f,15);
	PID_struct_init(&pid_cha_6020_speed[3], POSITION_PID, 15000, 500, 200,0.1f,10);//42,0.5f,20);

	for (int k = 0; k < 4; k++)
    {
		PID_struct_init(&pid_cha_3508_speed[k], POSITION_PID,15000, 4000,90,0.0f, 0); //24 0.3 10    38.0f,3.0f, 40
    }
//  PID_struct_init(&pid_chassis_angle, POSITION_PID, 600, 10, 150,0.05,50);//xisanhao
    PID_struct_init(&pid_chassis_angle, POSITION_PID, 6000, 10, 600,0,50);
	
	
}
//��Ҫ���ʿ��Ʋ���
#if POWER_LIMIT_HANDLE 
/**
************************************************************************************************************************
* @Name     : power_limit_handle
* @brief    : ���ʿ��Ƶ�������
* @param		: None
* @retval   : void
* @Note     : 
************************************************************************************************************************
**///����
u8 power_mode=0;
void power_limit_handle()
{
//		if(uart_cha_data.climbing_mode==1)
//		{
////			if(uart_cha_data.speed_mode==HIGH_SPEED_MODE)
////			{
////				if(usart_capacitance_message.cap_voltage_filte>1)
//						max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//						get_6020power();
//						power_limit_rate2=get_the_limite_rate(520);
//				 else
//				 {
//					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate(max_chassis_power-Max_Power_6020);
//					power_mode=1;
//			 }
//			else
//			{
//					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate(max_chassis_power-Max_Power_6020);
//					power_mode=1;
////			}
//		}
//		else
//		{
//				if(uart_cha_data.speed_mode==HIGH_SPEED_MODE)
//				{				
//					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate(max_chassis_power-Max_Power_6020);
//					power_mode=1;
//				}
//				else
//				{
//					
//				if(chassis.ctrl_mode==CHASSIS_ROTATE)
//				{
//					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate(max_chassis_power-Max_Power_6020);
//					power_mode=1;
//				}
//				else
//				{
//					max_chassis_power=get_max_power2(uart_cha_data.chassis_power_limit-2);
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate((uart_cha_data.chassis_power_limit-2)-Max_Power_6020);
//					power_mode=2;
//				}







				if(usart_capacitance_message.cap_voltage_filte>15)
				{
   					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
						get_6020power();
						power_limit_rate2=get_the_limite_rate(420);
					}
				else
				 {
					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
					power_limit_rate2=get_the_limite_rate(max_chassis_power-Max_Power_6020);
			   }

//				if(usart_capacitance_message.cap_voltage_filte>15)
//				{
//   					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//						get_6020power();
//						power_limit_rate2=get_the_limite_rate(420);
//					}
//				else
//				 {
//					max_chassis_power=get_max_power2(usart_capacitance_message.cap_voltage_filte);//(float)((usart_capacitance_message.cap_voltage_filte)-Max_Power_6020)
//					get_6020power();
//					power_limit_rate2=get_the_limite_rate(max_chassis_power);
//			   }
				 
				 
				 
				 
				 

	VAL_LIMIT(power_limit_rate1,0,1);
	VAL_LIMIT(power_limit_rate2,0,1);		 
		
}


/**
************************************************************************************************************************
* @Name     : get_max_power1  get_max_power2
* @brief    : ���ڻ�ȡ����ʿ���
* @param		: �������ݵ�ѹֵ
* @retval   : �����
* @Note     : get_max_power1/get_max_power2�沽
************************************************************************************************************************
**///������������

float max_power=0;

float get_max_power2(float voltage)
{
//	if(uart_cha_data.chassis_power_limit>voltage*16)
//	{max_power=uart_cha_data.chassis_power_limit;}
//	else
	{max_power=voltage*16-25;}
	//+uart_cha_data.chassis_power_limit;
//judge_rece_mesg.game_robot_state.chassis_power_limit+
//(judge_rece_mesg.power_heat_data.chassis_power_buffer-5)*2;

	VAL_LIMIT(max_power,0,voltage*16);//+uart_cha_data.chassis_power_limit);
	power_limit_rate1=1;
    return max_power;
}
/**
************************************************************************************************************************
* @Name     : get_the_limite_rate
* @brief    : ���ݸ��������������Ź�������ϵ��
* @param		: max_power
* @retval   : 
* @Note     :
************************************************************************************************************************
**/

static float get_the_limite_rate(float max_power)
{
	float a[4];
	for(int i=0; i<4; i++)
		a[i]=(float)chassis.cha_pid_3508.speed_ref[i] * (pid_cha_3508_speed[i].p+pid_cha_3508_speed[i].d);
	float b[4];
	for(int i=0; i<4; i++)
    b[i]=-pid_cha_3508_speed[i].p*(float)chassis.cha_pid_3508.speed_fdb[i]+pid_cha_3508_speed[i].iout \
         -pid_cha_3508_speed[i].d*(float)chassis.cha_pid_3508.speed_fdb[i]-pid_cha_3508_speed[i].d*pid_cha_3508_speed[i].err[LAST];
// Max_power=heat_power+drive_power
//	i_n=a[n]*k+b[n]	����
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
/**
************************************************************************************************************************
* @Name     : get_6020power
* @brief    : ���ݸ���������6020��������ϵ�� 
* @param		: ר���ڶ���
* @retval   : kall6020[i]
* @Note     : 
************************************************************************************************************************
**/
float get_6020power()
{
	for(int i=0;i<4;i++)
	{
		I6020[i] = pid_calc(&pid_cha_6020_speed[i], chassis.cha_pid_6020.speed_fdb[i], chassis.cha_pid_6020.speed_ref[i]);
		VAL_LIMIT(I6020[i],-10000,10000);	// 6020��ѹ�����ֵΪ25000
		dpower[i] = I6020[i]*pid_cha_6020_speed[i].get*k6020;
		hpower[i] = k2_6020*I6020[i]*I6020[i]+k1_6020*I6020[i]+k0_6020;
		all_power1[i] = dpower[i]+hpower[i];
	}
	all_power = all_power1[0]+all_power1[1]+all_power1[2]+all_power1[3];
	VAL_LIMIT(all_power,0,1000);
	Max_Power_6020=all_power;
	VAL_LIMIT(Max_Power_6020,0,max_chassis_power1);
	Max_Power_3508 = max_chassis_power-Max_Power_6020;

	float a[4],b[4],c[4];
	if(all_power>max_chassis_power1)
	{	
		for(int i = 0; i < 4; i++)
		{
			all_power2[i]=all_power1[i]*max_chassis_power1/all_power;
			if(all_power1[i]>k0_6020&&all_power1[i]>all_power2[i])//����all_power2>k0_6020����
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
#endif
/**
************************************************************************************************************************
* @Name     : limit_angle_to_0_2pi
* @brief    : ���Ƕ�������0~2pi��
* @param    : angle ���� float
* @retval   : angle ���� float
* @Note     : 
************************************************************************************************************************
**/
float limit_angle_to_0_2pi(float angle)
{
		if (angle>=2*PI)angle -= 2*PI;
		else if(angle<0)angle += 2*PI;
	
	return angle;
}
/**
************************************************************************************************************************
* @Name     : convert_ecd_angle_to_0_2pi
* @brief    : ������������Ļ�е�Ƕ�ֵ����Χ��������󣩽���Ϊ��Χ��0~2pi�ĽǶ�ֵ      
* @param		: ecd_angle ����������Ļ�е�Ƕ�ֵ  ����  double
* @param		: _0_2pi_angle ��Χ��0~2pi�ĽǶ�ֵ  ����  float
* @retval   : _0_2pi_angle ��Χ��0~2pi�ĽǶ�ֵ  ����  float
* @Note     : 
************************************************************************************************************************
**/
double convert_ecd_angle_to_0_2pi(double ecd_angle,float _0_2pi_angle)
{
	_0_2pi_angle=fmod(YAW_POLARITY*ecd_angle*ANGLE_TO_RAD,2*PI);	
	if(_0_2pi_angle<0)
		 _0_2pi_angle+=2*PI;

	return _0_2pi_angle;
}

/**********************
*@Brief:�����źŷ�����
*@Call:�ڲ����ⲿ
*@Param:T:������������
				f�������ź�Ƶ��
*@Note:��Χ-1~1
*@RetVal:��
**********************/
float Sinusoidal_Waveform_Generator_1(float T,float f)
{
	float static t,CNT,Value,w;
	
	CNT++;
	
	w=2*3.14159*f;
	
	t=CNT*T;//ʱ��
	Value=sin(w*t);
	
	if(w*t>=2*3.14)
		t=0;
	
	return Value;
}

/**
************************************************************************************************************************
* @Name     : 
* @brief    : chassis_mode_select�����µĸ�ʵ�ֺ���
* @retval   : 
* @Note     : ����
*							chassis_stop_handle          ���̹ؿ�				
*							follow_gimbal_handle         ���̸�����̨ģʽ���� ��Ҫ��vx vy vw�Ļ�ȡ
*							separate_gimbal_handle       ������̨����ģʽ����
*							rotate_follow_gimbal_handle	 С����ģʽ����
*             reverse_follow_gimbal_handle С���ݷ�תģʽ����
************************************************************************************************************************
**/

u16 sin_k = 350;
void chassis_stop_handle(void)
{
  chassis.vy = 0;
  chassis.vx = 0;
	chassis.vw = 0;
  Chassis_angle.get_speedw = 0;//����ʹ��
}

void follow_gimbal_handle(void)
{
	
		if(Chassis_angle.yaw_angle_0_2pi>=PI)
		{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi-(2*PI);	}
		else
		{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi;	}

	chassis.vy = uart_cha_data.y;
	chassis.vx = uart_cha_data.x;

//	Chassis_angle.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0);   // ����������
		/* ʸ�� */
		if(uart_cha_data.speed_mode == HIGH_SPEED_MODE)
		{
			pid_chassis_angle_deathband = 0.0f;
		}
		else
		{
			pid_chassis_angle_deathband = 0;//15.0f * 3.14f / 180.0f;
		}
	Chassis_angle.get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);
}


void separate_gimbal_handle(void)
{
  chassis.vy = uart_cha_data.y;
  chassis.vx = uart_cha_data.x;
	Chassis_angle.get_speedw = 0;
}

void rotate_follow_gimbal_handle(void)
{
	if(Chassis_angle.yaw_angle_0_2pi>=PI)
	{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi-(2*PI);}
	else
	{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi;}

					if(uart_cha_data.speed_mode==HIGH_SPEED_MODE&&chassis.vx==0&&chassis.vy==0)
					{
						gyro_speed=(2.2-0.4*Sinusoidal_Waveform_Generator_1(0.002/*�ú���ִ������*/,0.33/*�������Ҳ�Ƶ��*/))*sin_k;
					}
					else
					{
						if(judge_rece_mesg.game_robot_state.robot_level==1)
						gyro_speed=500;		//200
						if(judge_rece_mesg.game_robot_state.robot_level==2)
						gyro_speed=500;
						if(judge_rece_mesg.game_robot_state.robot_level==3)
						gyro_speed=500;
						if(judge_rece_mesg.game_robot_state.robot_level==4)
						gyro_speed=550;
						if(judge_rece_mesg.game_robot_state.robot_level==5)
						gyro_speed=550;
						if(judge_rece_mesg.game_robot_state.robot_level==6)
						gyro_speed=600;
						if(judge_rece_mesg.game_robot_state.robot_level==7)
						gyro_speed=600;
						if(judge_rece_mesg.game_robot_state.robot_level==8)
						gyro_speed=650;
						if(judge_rece_mesg.game_robot_state.robot_level==9)
						gyro_speed=700;
						if(judge_rece_mesg.game_robot_state.robot_level==10)
						gyro_speed=700;
					}

	chassis.vy = uart_cha_data.y;
	chassis.vx = uart_cha_data.x;
		
	if(chassis.last_ctrl_mode!=CHASSIS_ROTATE)
	{
		if(get_speedw_flag==0)
		{get_speedw_flag=1;}
		else if(get_speedw_flag==1)
		{get_speedw_flag=0;}
	}

	if(get_speedw_flag==0)
	{Chassis_angle.get_speedw = -gyro_speed;}
	else if(get_speedw_flag==1)
	{Chassis_angle.get_speedw = gyro_speed;}

}

void reverse_follow_gimbal_handle(void)
{
	chassis.vy = uart_cha_data.y;
	chassis.vx = uart_cha_data.x;

	if(Chassis_angle.yaw_angle_0_2pi>=2*PI)
	{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi-(2*PI);	}
	else
	{	Chassis_angle.yaw_angle__pi_pi=Chassis_angle.yaw_angle_0_2pi;}
		
//   Chassis_angle.get_speedw = pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,PI);
	Chassis_angle.get_speedw = pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);    
}

/**
************************************************************************************************************************
* @Name     :start_angle_handle
* @brief    :�������ʼ�ǣ�ƫ���ǣ��������ٶȣ��Լ����Ƽ��������ٶ�
* @param		:None 
* @retval   :void
* @Note     :���빦�����Ʊ�־λ���������ٶȸ��� ��ʵ�ʷ��͵�ֵ	
							10.3 ����transition3Ϊnan�����
************************************************************************************************************************
**/
/**********************
*@Brief:���ֽ��㺯��
*@Call:�ڲ����ⲿ
*@Param:
*@Note:��
*@RetVal:��
**********************/
int Speed_Polarity[4];
void Chassis_Helm_Cal(void)
{
	float static Dirction_Angle_Medium,Heading_Angle_Medium[4],Heading_Speed_Medium[4];
	Dirction_Angle_Medium=Chassis_angle.yaw_angle__pi_pi;
	
	float static V_x_Medium[4],V_y_Medium[4];
	for(int i=0;i<4;i++)
	{
		/**************************�����������***************************/
		V_x_Medium[i]=chassis.vx*cos(-Dirction_Angle_Medium)-chassis.vy*sin(-Dirction_Angle_Medium)-Chassis_angle.get_speedw*cos((135+i*90)*3.14/180);
		V_y_Medium[i]=chassis.vx*sin(-Dirction_Angle_Medium)+chassis.vy*cos(-Dirction_Angle_Medium)-Chassis_angle.get_speedw*cos((45+i*90)*3.14/180);
		
		Heading_Angle_Medium[i]=atan2(V_y_Medium[i],V_x_Medium[i])*180/3.14;
		Heading_Speed_Medium[i]=sqrt(V_y_Medium[i]*V_y_Medium[i]+V_x_Medium[i]*V_x_Medium[i]);
		/*************************�����������***************************/
	}
	for(uint8_t i=0; i<4; i++)
	{
		Chassis_angle.handle_speed_lim[i] = Heading_Speed_Medium[i];
		Chassis_angle.deviation_angle[i] = Heading_Angle_Medium[i];		
	}
}

float watcha;
float watchb;
/**
 * @brief  ���� ֱ�� ʸ���ϳ� �Ķ��ֽ���
 * */
void steering_wheel_calc2(void)
{
	float vx = 0;
	float vy = 0;
	float vx_last = 0;
	float vy_last = 0;
	float handle_speed_lim_last[4] = {0.0f};
	float relative_angle = -Chassis_angle.yaw_angle__pi_pi;
	/* С���ݽǶȲ��� */
	if(chassis.ctrl_mode==CHASSIS_ROTATE)
	{
		if(get_speedw_flag == 1)
			relative_angle += 5.0f *ANGLE_TO_RAD;		//13 6.5
		else if(get_speedw_flag == 0)
			relative_angle -= 20.0f *ANGLE_TO_RAD;		//13 6.5
	}
	for(uint8_t i = 0; i < 4; i++)
	{
		vx_last = vx;
		vy_last = vy;
		vx = chassis.vx*cosf(relative_angle) - chassis.vy*sinf(relative_angle) + Chassis_angle.get_speedw*cosf(-1.0f*PI/4.0f + i*PI/2.0f);
		vy = chassis.vx*sinf(relative_angle) + chassis.vy*cosf(relative_angle) + Chassis_angle.get_speedw*sinf(-1.0f*PI/4.0f + i*PI/2.0f);
		
		handle_speed_lim_last[i] = Chassis_angle.handle_speed_lim[i];
		Chassis_angle.handle_speed_lim[i] = sqrt(vx*vx + vy*vy);
		
//		if(fabs(vx_last - vx) > 3000.0f || fabs(vy_last - vy) > 4000.0f)
//			Chassis_angle.deviation_angle[i] = 90;
//		else
			Chassis_angle.deviation_angle[i] = (float)atan2(vy, vx)*180/3.14;
	}
	watcha = vx; watchb = vy;
//	for(uint8_t i = 0; i < 4; i++)
//	{
//		if(fabs(chassis.vx) == 0 && fabs(chassis.vy) == 0)
//		{
//			Chassis_angle.handle_speed_lim[i] = 0;
//			Chassis_angle.deviation_angle[i] = 90;
//		}
//	}
}

double K_Helm=1.1;
void start_angle_handle()
{
//	steering_wheel_calc(10*K_Helm,10*K_Helm);
  steering_wheel_calc2();
//	Chassis_Helm_Cal();
}

float sL,lL,sW,lW;

/**
 * @brief  ���� �ĸ������� ��Բ�� Բ���˶� �Ľ���
 * @param  Length�� ��  ����
 * @param  Weight�� ��  ����
 * */
void steering_wheel_calc(double Length,double Weight)
{
		
		if(Chassis_angle.get_speedw!=0)
			R=Chassis_angle.Remote_speed/fabs(Chassis_angle.get_speedw);
		else
			R=0;

	//short_Length,long_Length,short_Width,long_Width

		sL= -Length/2+R*sinf(theta);
		lL=  Length/2+R*sinf(theta);
		sW=	-Weight/2+R*cosf(theta);
		lW=  Weight/2+R*cosf(theta);
		
	//transition1�Ǹ���ͬԲ�뾶Ri��transition2�Ǹ���ƫ����Ai 
	//0��ǰ�� 1��ǰ�� 2����� 3�Һ��� ��
	//ң�ؽǹ��ڵ�һ����
		if(chassis.ctrl_mode==CHASSIS_ROTATE)
		{
			if(get_speedw_flag == 1)
				theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi-PI/15;
			else if(get_speedw_flag == 0)
				theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi+PI/15;
		}
		else
        {
					
//      theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi;//����������
					
            if(Chassis_angle.yaw_angle__pi_pi>-PI/4&&Chassis_angle.yaw_angle__pi_pi<=PI/4)
            {
            Chassis_angle.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0);   // ����������
//				Chassis_angle.get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);  
				theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi;//����������
            }
            else if(Chassis_angle.yaw_angle__pi_pi>PI/4&&Chassis_angle.yaw_angle__pi_pi<=3*PI/4)
            {
            Chassis_angle.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,PI/2);   // ����������
//				Chassis_angle.get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);  
			theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi+2*PI;//����������
            } 
            else if(Chassis_angle.yaw_angle__pi_pi>=3*PI/4||Chassis_angle.yaw_angle__pi_pi<-3*PI/4)
            {
            Chassis_angle.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle_0_2pi,PI);   // ����������
//				Chassis_angle.get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);  
			theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi+2*PI;//����������
            }
            else
            {
            Chassis_angle.get_speedw = -pid_calc(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,-PI/2);   // ����������
//			Chassis_angle.get_speedw = -pid_calc_filter(&pid_chassis_angle,Chassis_angle.yaw_angle__pi_pi,0, pid_chassis_angle_deathband);  
			theta = Chassis_angle.Remote_angle-Chassis_angle.yaw_angle_0_2pi-2*PI;//����������  
				
            }
        }
        
		
			if(Chassis_angle.Remote_angle>=(PI/2)&&Chassis_angle.Remote_angle<PI)
			theta-=PI/2;
			else if(Chassis_angle.Remote_angle>=PI&&Chassis_angle.Remote_angle<(3*PI/2))
			theta-=PI;
			else if(Chassis_angle.Remote_angle>=(3*PI/2)&&Chassis_angle.Remote_angle<(2*PI))
			theta-=3*PI/2;//�߽������ǵô���
			
            
            
            
			//���ֵ���˳��
			//���ڵ�һ����
		
			transition1[1]=sqrt(lW*lW+lL*lL);
			transition1[2]=sqrt(sW*sW+lL*lL);
			transition1[3]=sqrt(sW*sW+sL*sL);
			transition1[0]=sqrt(lW*lW+sL*sL);
		
			transition2[1]=atan2(lL,lW);
			transition2[2]=atan2(lL,sW);		
			transition2[3]=atan2(sL,sW);
			transition2[0]=atan2(sL,lW);

			if(Chassis_angle.Remote_angle>=(PI/2)&&Chassis_angle.Remote_angle<PI)
			{					
				retransition1[0]=transition1[3];
				retransition1[1]=transition1[0];
				retransition1[2]=transition1[1];
				retransition1[3]=transition1[2];
				
				retransition2[0]=transition2[3];
				retransition2[1]=transition2[0];
				retransition2[2]=transition2[1];
				retransition2[3]=transition2[2];
				
				for(int i=0;i<4;i++)
				{
					transition1[i]=retransition1[i];
					transition2[i]=retransition2[i]+PI/2;
				}						
			}
			
			if(Chassis_angle.Remote_angle>=PI&&Chassis_angle.Remote_angle<(3*PI/2))
			{		
				retransition1[0]=transition1[2];
				retransition1[1]=transition1[3];
				retransition1[2]=transition1[0];
				retransition1[3]=transition1[1];
				
				retransition2[0]=transition2[2];
				retransition2[1]=transition2[3];
				retransition2[2]=transition2[0];
				retransition2[3]=transition2[1];
				
				for(int i=0;i<4;i++)
				{
					transition1[i]=retransition1[i];
					transition2[i]=retransition2[i]+PI;
				}						
			}
			
			if(Chassis_angle.Remote_angle>=(3*PI/2)&&Chassis_angle.Remote_angle<(2*PI))
			{		
				retransition1[0]=transition1[1];
				retransition1[1]=transition1[2];
				retransition1[2]=transition1[3];
				retransition1[3]=transition1[0];
				
				retransition2[0]=transition2[1];
				retransition2[1]=transition2[2];
				retransition2[2]=transition2[3];
				retransition2[3]=transition2[0];
				
				for(int i=0;i<4;i++)
				{
					transition1[i]=retransition1[i];
					transition2[i]=retransition2[i]+3*PI/2;
				}						
			}
					
		 if(Chassis_angle.get_speedw<0)//�������
		{
			retransition1[0]=transition1[2];
			retransition1[1]=transition1[3];
			retransition1[2]=transition1[0];
			retransition1[3]=transition1[1];
			
			retransition2[0]=transition2[2];
			retransition2[1]=transition2[3];
			retransition2[2]=transition2[0];
			retransition2[3]=transition2[1];
		
			for(int i=0;i<4;i++)
			{
				transition1[i]=-retransition1[i];
				transition2[i]=retransition2[i];
			}		
		}

		
		 if(Chassis_angle.get_speedw==0&&(chassis.vx!=0||chassis.vy!=0))
		{			
			transition1[0]=1;
			transition1[1]=1;
			transition1[2]=1;
			transition1[3]=1;

			transition2[0]=Chassis_angle.yaw_angle__pi_pi;
			transition2[1]=Chassis_angle.yaw_angle__pi_pi;
			transition2[2]=Chassis_angle.yaw_angle__pi_pi;
			transition2[3]=Chassis_angle.yaw_angle__pi_pi;	
		}
		
		for(int i=0;i<4;i++)
		{
//			Chassis_angle.handle_speed_lim[i] = transition1[i]*Chassis_angle.get_speedw*0.22f;
      Chassis_angle.handle_speed_lim[i] = transition1[i]*Chassis_angle.get_speedw*0.4f;
		  Chassis_angle.deviation_angle[i]  = transition2[i]*RAD_TO_ANGLE;		
		}
		
}
/**
************************************************************************************************************************
* @Name     : start_chassis_6020
* @brief    : ��������pid�ٶȻ�/�ǶȻ� ����ֵ�Ļ�ȡ������ֵ������
* @param    : none
* @retval   : void 
* @Note     : ת�ӻ��Ĵ���
************************************************************************************************************************
**///����

void start_chassis_6020()
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
		{
			K_A[i]-=2;
		}
		else if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])<-90&&(chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])>-270)
		{
			K_A[i]+=1;
			K_S[i]=-K_S[i];
		}
		else if((chassis.cha_pid_6020.angle_ref[i]-chassis.cha_pid_6020.angle_fdb[i])<-270)
		{
			K_A[i]+=2;
		}
 	}
	
	for(int i=0;i<4;i++)
		pid_calc(&pid_cha_6020_angle[i],chassis.cha_pid_6020.angle_fdb[i],chassis .cha_pid_6020.angle_ref[i]);

	if(chassis .ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE)
	{
		for(int j=0;j<4;j++)
		chassis.cha_pid_6020.speed_fdb[j]=-steering_wheel_chassis.Heading_Encoder[j].filter_rate;
		
		for(int i=0;i<4;i++)
		chassis.cha_pid_6020.speed_ref[i]=pid_cha_6020_angle[i].out;
	}
}

/**
************************************************************************************************************************
* @Name     :set_3508current_6020voltage
* @brief    :�����ֵ���ٶȻ�������ȡ�͸������ã������ֺͺ�������pid���ո������㴦��
* @param    :None
* @retval   :void
* @Note     :���ʿ����л�����
************************************************************************************************************************
**/
/* ֹͣʱ�� */
uint32_t stop_time;
void set_3508current_6020voltage()
{		
	/* 6020 speed ref ���� */
	start_chassis_6020();
	/* �ٶȻ�pid���� + �������� */
	for (int i = 0; i < 4; i++)
	{ 
		chassis.cha_pid_3508.speed_fdb[i]=steering_wheel_chassis.Driving_Encoder[i].filter_rate;
		chassis.cha_pid_3508.speed_ref[i]=Chassis_angle.handle_speed_lim[i];

		if(Chassis_angle.Remote_speed==0&&fabs(Chassis_angle.get_speedw)<5)
			chassis.cha_pid_3508.speed_ref[i]=0;
			
		#if POWER_LIMIT_HANDLE
		pid_calc(&pid_cha_6020_speed[i],chassis.cha_pid_6020.speed_fdb[i],  chassis.cha_pid_6020.speed_ref[i]);//kall6020[i] *
		pid_calc(&pid_cha_3508_speed[i],chassis.cha_pid_3508.speed_fdb[i] , power_limit_rate2 * chassis.cha_pid_3508.speed_ref[i]); //*power_limit_rate1
		#else
		pid_calc(&pid_cha_6020_speed[i],chassis.cha_pid_6020.speed_fdb[i],chassis.cha_pid_6020.speed_ref[i]);
		pid_calc(&pid_cha_3508_speed[i],chassis.cha_pid_3508.speed_fdb[i],chassis.cha_pid_3508.speed_ref[i]);
		#endif	
		/* ȥ����ɲ */		
		if(chassis.vx == 0 && chassis.vy == 0  && fabs(Chassis_angle.get_speedw) < 50)
		{
			stop_time ++;
			if(stop_time < 500)
			{	/*������*/
				for(uint8_t i = 0; i < 4; i++)
				{
					pid_cha_6020_speed[i].out = 0;
				}
			}
		}
		else if(!(chassis.vx == 0 && chassis.vy == 0 ))
		{
			stop_time = 0;
		}
	}
	/* �������� */
	for(int i = 0; i < 4; i++)
	{   
		if((chassis.ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE))				
				chassis.current[i] = 1.0f * pid_cha_3508_speed[i].out;		
		else
			{
			chassis.current[i]=0;
			}
		}			

		if(abs(chassis.cha_pid_3508.speed_ref[0])<50&&
			 abs(chassis.cha_pid_3508.speed_ref[1])<50&&
			 abs(chassis.cha_pid_3508.speed_ref[2])<50&&
			 abs(chassis.cha_pid_3508.speed_ref[3])<50)
		{
			run_flag=0;
		}
		else
		{
			run_flag=1;
		}
		for (int i = 0; i < 4; i++)
		{   
			if((chassis.ctrl_mode==MANUAL_FOLLOW_GIMBAL||chassis.ctrl_mode==CHASSIS_ROTATE))
				chassis.voltage[i]=1.0f*(int16_t)pid_cha_6020_speed[i].out*STEERING_POLARITY;	
			else
				chassis.voltage[i]=0;
		}
}
/**
************************************************************************************************************************
* @Name     : get_remote_set
* @brief    : ����ң�����ٶȷ���ʹ�С
* @param    : None
* @retval   : void
* @Note     : 10.3 �Ż����㷨����ȷ����ˢ����
************************************************************************************************************************
**/
void get_remote_set()
{	
//��ʱ������	
	
	Chassis_angle.yaw_encoder_ecd_angle=uart_cha_data.yaw_Encoder_ecd_angle;
	Chassis_angle.yaw_angle_0_2pi       =convert_ecd_angle_to_0_2pi(Chassis_angle.yaw_encoder_ecd_angle,Chassis_angle.yaw_angle_0_2pi);
	
	float temp_angle=0;
		vx = uart_cha_data.x*1.3;//vx��x�Ǻ���
		vy = uart_cha_data.y*1.3;//vy��y������
		Chassis_angle.Remote_speed = sqrt((vx*vx)+(vy*vy));
		if(Chassis_angle.Remote_speed >= 50)
		{	
			temp_angle=atan2(vy,vx);
			Chassis_angle.Remote_angle = fmod(2*PI+temp_angle,2*PI);
		}
		else
		{Chassis_angle.Remote_speed = 0;}
}
/**
************************************************************************************************************************
* @Name     : chassis_mode_select
* @brief    : ����λ�����͵����ѡ������ʵ��
* @param		: none
* @retval   : void
* @Note     : 
************************************************************************************************************************
**/
void chassis_mode_select(void)
{
	chassis.ctrl_mode=uart_cha_data.chassis_mode;
	
	
	switch (chassis.ctrl_mode)
    {
		case CHASSIS_STOP:
		{
			chassis_stop_handle();
		}
		break;

		case MANUAL_FOLLOW_GIMBAL:   //������̨ģʽ
		{
			follow_gimbal_handle();
		}
		break;
		case CHASSIS_ROTATE:         //С����
		{
			rotate_follow_gimbal_handle();
		}
		break;
		case CHASSIS_REVERSE:
		{
			reverse_follow_gimbal_handle();
		}
		break;
		case CHASSIS_SEPARATE:
		{
			separate_gimbal_handle();
		}
		break;
		default:
		{
			chassis_stop_handle();
		}
		break;
    }
	chassis.last_ctrl_mode=uart_cha_data.chassis_mode;
}

void Chassis_PID_handle(void)
{
	#if POWER_LIMIT_HANDLE
		power_limit_handle();	
	#endif
		set_3508current_6020voltage();
}

void Motion_resolution(void)
{
		get_remote_set();
		start_angle_handle();
}

/**
************************************************************************************************************************
* @Name     : chassis_task
* @brief    : ģʽѡ��->�˶�����->��������
* @retval   : None
* @Note     : 
************************************************************************************************************************
**/
void chassis_task()
{
	chassis_mode_select();
	Motion_resolution();
	Chassis_PID_handle();
}


//		PID_struct_init(&pid_cha_6020_angle[0], POSITION_PID, 8000, 10, 7,0.1f,5);//24+ 0.2f,20);
//  	PID_struct_init(&pid_cha_6020_speed[0], POSITION_PID, 15000, 500, 210,0.1f,10);//38,0.5f,20);
//	
//		PID_struct_init(&pid_cha_6020_angle[1], POSITION_PID, 8000, 10, 7,0.1f,5);//25, 0.2f,15);
//  	PID_struct_init(&pid_cha_6020_speed[1], POSITION_PID, 15000, 500, 180,0.1f,4);//39,0.5f,20);
//	
//	  PID_struct_init(&pid_cha_6020_angle[2], POSITION_PID, 8000, 10, 7,0.1f,4);//20, 0.2f,20);
//  	PID_struct_init(&pid_cha_6020_speed[2], POSITION_PID, 15000, 500, 200,0.5,8);//40,0.5f,20);
//		
//    PID_struct_init(&pid_cha_6020_angle[3], POSITION_PID, 8000, 10, 7,0.4f,4);//23, 0.2f,15);
//  	PID_struct_init(&pid_cha_6020_speed[3], POSITION_PID, 15000, 500, 200,0.1f,20);//42,0.5f,20);

