#ifndef __CHASSIS_TASK_H 
#define __CHASSIS_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "public.h"
/*----------------------------------------------------------------------------*/
#define POWER_LIMIT_HANDLE   1
#define P3508 60//judge_rece_mesg.game_robot_state.chassis_power_limit
/*******************************CONFIG********************************/
#define YAW_POLARITY 					-1 //逆正      Yaw轴电机极性 舵轮要顺正，改-1；麦轮1

#define RIGHT_FRONT_REVERSE   1 
#define LEFT_FRONT_REVERSE    -1
#define LEFT_BEHIND_REVERSE   -1
#define RIGHT_BEHIND_REVERSE  -1
#define  WARNING_VOLTAGE       12.5
#define STEERING_POLARITY      -1 //底盘四个6020电机的输出极性 解算不考虑 故置-1
/*******************************CONFIG********************************/
//M3508
#define  I_TIMES_V_TO_WATT    0.0000225f    //I -16384~+16384 V .filter_rate
//电机发热计算 p=i^2*FACTOR_2+i*FACTOR_1+FACTOR0; i是直接发给电调的数-16384~16384 使用虚拟示波器读值后matlab拟合
#define FACTOR_2	0.000000161f
#define FACTOR_1	-0.0000229f
#define FACTOR_0  0.458f

//GM6020(西交模型)
#define CURRENT_TO_T 0.741*(3.0f/16384) //转矩常数*（最大电流/最大转矩电流值）
#define K_WT         1/9.55f         //伺服电机系数
#define K1           0.0003723//0.0003723
#define K2           31.99//33.14    
#define K3           1.304//0.5071

#define CHASSIS_DEFAULT { \
				.vx=0, \
				.vy=0, \
				.vw=700, \
				.ctrl_mode=0, \
				.last_ctrl_mode=0, \
				.chassis_speed_mode=0, \
	      .chassis_power.Max_Steering_Power=35, \
				.current={0}, \
				.voltage={0}, \
				.cha_pid_6020.angle_ref={0}, \
				.cha_pid_6020.angle_fdb={0}, \
				.cha_pid_3508.angle_ref={0}, \
				.cha_pid_3508.angle_fdb={0}, \
				.cha_pid_6020.speed_ref={0}, \
				.cha_pid_6020.speed_fdb={0}, \
				.cha_pid_3508.speed_ref={0}, \
				.cha_pid_3508.speed_fdb={0}, \
				.Init   = &chassis_param_init, \
				.STOP_  = &chassis_stop_handle, \
				.FOLLOW =&follow_gimbal_handle, \
				.ROTATE =&rotate_follow_gimbal_handle, \
				.Mode_Select= &chassis_mode_select, \
				.Power_Limit= &power_limit_handle, \
				.PID_Calc=&Chassis_PID_handle, \
} \

#define CHASSIS_ANGLE_DEFAULT { \
				.vx=0, \
				.vy=0, \
				.Remote_speed=0, \
				.yaw_angle_0_2pi=0, \
				.yaw_angle__pi_pi=0, \
				.yaw_encoder_ecd_angle=0, \
				.deviation_angle={0}, \
				.handle_speed={0}, \
				.handle_speed_lim={0}, \
				.Get_Yaw_Data =&convert_ecd_angle_to_0_2pi, \
				.Get_RC_Data  =&get_remote_set, \
				.Get_Steer_Para =&steering_wheel_calc, \
				.Motion_Soving=&Motion_resolution, \
} \


/**
************************************************************************************************************************
* @EnumName : chassis_mode_e
* @brief    : This enumeration describes the various control modes of the chassis     
* @Note     : 					
************************************************************************************************************************
**/
typedef enum
{
  CHASSIS_RELAX          = 0,
  CHASSIS_STOP           = 1,
  MANUAL_SEPARATE_GIMBAL = 2,
  MANUAL_FOLLOW_GIMBAL   = 3,
  DODGE_MODE             = 4,
  AUTO_SEPARATE_GIMBAL   = 5,
  AUTO_FOLLOW_GIMBAL     = 6,
  CHASSIS_ROTATE         = 7,
  CHASSIS_REVERSE        = 8,
  CHASSIS_CHANGE_REVERSE = 9,
  CHASSIS_SEPARATE 		 	 = 10,
  CHASSIS_AUTO_SUP       = 11,
} chassis_mode_e;

typedef enum
{
  NORMAL_SPEED_MODE          = 2,
	HIGH_SPEED_MODE            = 3,
	LOW_SPEED_MODE             = 1,
} chassis_speed_mode_e;

typedef __packed struct 
{
	uint8_t         climbing_mode;
	uint8_t         follow_gimbal;
	uint8_t  			 get_speedw_flag;
	uint8_t 				 two_wheel_mode;
} chassis_flag_t;

typedef __packed struct
{
	float power_limit_rate;
    float power_T_limlit_rate;
    float power_6020_limlit_rate;
	float Cap_V;
	float Max_Chassis_Power;
	float Max_Steering_Power;
	float Max_Power_3508;
	float Max_Power_6020;
    
    float steer_prepower;
    float driving_prepower;
} chassis_power_t;
/**
************************************************************************************************************************
* @StructName : cha_pid_t
* @brief    	: This enumeration describes the various control modes of the chassis
* @param    	: anglex_ref    		
*	@param			:	anglex_fdb			
* @param			: speedx_ref 		
* @param			: speedx_fdb 		 		
* @Note    		: 					
************************************************************************************************************************
**/
typedef __packed struct
{
  /* position loop */

	float angle_ref[4];
	float angle_fdb[4];
  /* speed loop */

	int16_t speed_ref[4];
	int16_t speed_fdb[4];
	
} cha_pid_t;

/**
************************************************************************************************************************
* @StructName : cha_pid_t
* @brief    	: This enumeration describes the various control modes of the chassis
* @param    	: anglex_ref    		
*	@param			:	anglex_fdb			
* @param			: speedx_ref 		
* @param			: speedx_fdb 		 		
* @Note    		: 					
************************************************************************************************************************
**/
typedef struct chassis_t
{
		float           vx,vy,vw,vx_ramp,vy_ramp; 
		float           get_speedw;							///*给定机体转动角速度*/	
		chassis_mode_e  			 ctrl_mode,last_ctrl_mode;			 
		chassis_speed_mode_e  chassis_speed_mode;
		chassis_power_t				 chassis_power;
		chassis_flag_t         chassis_flag;
		cha_pid_t       				 cha_pid_6020;
		cha_pid_t			 				 cha_pid_3508;
        
        pid_t pid_cha_6020_angle[4];
        pid_t pid_cha_3508_angle[4];
        pid_t pid_cha_6020_speed[4];
        pid_t pid_cha_3508_speed[4];
        
        pid_t pid_cha_6020c_angle[4];
        pid_t pid_cha_6020c_speed[4];

		int16_t         				 current[4];
        int16_t                         GM_current[4];                
		int16_t				 		 voltage[4];

		void (*Init)	 (struct chassis_t *chassis);
		void (*STOP_) (struct chassis_t *chassis);
		void (*FOLLOW) (struct chassis_t *chassis);
		void (*ROTATE) (struct chassis_t *chassis);
		void (*Mode_Select) (struct chassis_t *chassis);
	  void (*Power_Limit) (struct chassis_t *chassis);
		void (*PID_Calc) (struct chassis_t *chassis);
	
}chassis_t;
	
/**
************************************************************************************************************************
* @StructName : cha_pid_t
* @brief    	: This enumeration describes the various control modes of the chassis
* @param    	: anglex_ref    		
*	@param			:	anglex_fdb			
* @param			: speedx_ref 		
* @param			: speedx_fdb 		 		
* @Note    		: 					
************************************************************************************************************************
**/
typedef __packed struct Chassis_angle_ /*一定要在上面加这个，不然在调用函数时出现struct类型不符错误*/
{			
		float vx,vy;										///*遥控器获取参数*/
		float Remote_angle;						///*遥控器给定角度*/
		float Remote_speed;						///*遥控器给定速度*/
		float deviation_angle[4];		///*给定舵向电机角度*/
		int16_t handle_speed[4];
		int16_t handle_speed_lim[4];///*给定驱动电机速度*/

		float yaw_angle_0_2pi;				///*yaw轴朝向角度 范围0-2PI*/
		float yaw_angle__pi_pi;				///*yaw轴朝向角度 范围-PI-PI*/
		double yaw_encoder_ecd_angle;///*yaw轴机械角度 范围-∞-+∞*/

	  void (*Get_Yaw_Data) (struct Chassis_angle_ *chassis_angle,double ecd_angle);/*获取Yaw轴数据*/
		void (*Get_RC_Data) (struct Chassis_angle_ *chassis_angle);/*获取遥控器数据*/
	  void (*Get_Steer_Para) (struct Chassis_angle_ *chassis_angle);/*舵轮解算*/
		void (*Motion_Soving) (struct Chassis_angle_ *chassis_angle,double ecd_angle);
}Chassis_angle_t;


extern Chassis_angle_t 	 Chassis_angle;
extern chassis_t 		 		 chassis;
extern float test_in,test_out;
void power_limit_handle(chassis_t *chassis);
void convert_ecd_angle_to_0_2pi(Chassis_angle_t *chassis_angle,double ecd_angle);
void Motion_resolution(Chassis_angle_t *chassis_angle,double ecd_angle);
void Chassis_PID_handle(chassis_t *chassis);
void chassis_param_init(chassis_t *chassis);
void chassis_task(void);
void steer_optimizing(void);
void get_remote_set(Chassis_angle_t *chassis_angle);
void start_angle_handle(void);
void start_chassis_6020(void);
float get_6020power(void);
float get_6020_T_limit_rate(float max_power);
void start_chassis_3508(void);
float get_max_power2(float voltage);
float get_max_power1(float voltage);
static float get_the_V_limite_rate(float max_power);
static float get_T_limit_rate(float max_power);
void cap_limit_mode_switch(void);
void chassis_mode_select(chassis_t *chassis);
void chassis_stop_handle(chassis_t *chassis);
void follow_gimbal_handle(chassis_t *chassis);
void rotate_follow_gimbal_handle(chassis_t *chassis);
void buffer_power(void);
float get_max_power(float voltage);
void start_chassis_6020C(void);
void steering_wheel_calc(Chassis_angle_t *chassis_angle); 
#endif



