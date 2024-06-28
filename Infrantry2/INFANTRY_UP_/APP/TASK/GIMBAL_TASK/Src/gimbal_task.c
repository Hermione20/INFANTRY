#include "gimbal_task.h"

/**
  ******************************************************************************
  * @file    gimbal_task.c
  * @author  Lee_ZEKAI
  * @version V1.1.0
  * @date    03-October-2023
  * @brief   ��ģ��Ϊͨ����̨ģ�飬��������λ��Դ�ļ��ϲ��ֺ�ͷ�ļ��ϲ��֣�
						 Դ�ļ�������id��1��Ӣ�ۣ�2�����̣�3-4-5��������6���ɻ���7���ڱ���
						 
										��̨��λ����
										�Ӿ���̨��λ
										��̨��ʼ����������
										��ͨģʽ��̨��������
										����������̨��������
										��������������
										����ǶȲ���
										pid����
						 ͷ�ļ���������ֲ�������
						 
	* @notice  ��ģ��ͨ�������б��֣���δ���Ĵ���ά�����뿪����Աά��ģ���
						 �����ԣ�ά������̨���ͨ���ԣ���ֹ��������̨���ֵ��߼������
						 д�����ģ���ڣ���ģ���������̨�Ŀ��ƣ��ο�����ĸ�ֵ���Ʋ�
						 ģʽѡ��
						 
	* @notice  ��̨ģ��ĵ������Ʋ���control_task���Ƽ���̨����Ƶ��Ϊ2ms
						 �Ƽ���̨�������Ƶ��Ϊ2ms����controltask�����gimbal_task
						 ��control_task_Init�����gimbal_parameter_Init
						 
	*	@introduction ��ģ�����״̬���ķ�ʽ��д��̨�ĸ���ģʽ�빦�ܣ������ź�
									��������ģʽ���л���ѡ������mode_switch_tasks��ȫģ���
									������gimbal_t�ṹ�������ģ����Զ���״̬�۲����ĸ�����
									Դ����Ӧ�ӿ�Ϊ�궨���βΪ_FDB���ɵ��ÿ�ܵ�ͨ�ô�������
									��������磺
									#define YAW_INIT_ANGLE_FDB          -yaw_Encoder.ecd_angle
									
 ===============================================================================
 **/
 /**
  ******************************************************************************
																			��������
		����id��1��Ӣ�ۣ�2�����̣�3-4-5��������6���ɻ���7���ڱ���
		
		��̨��λ����
		�Ӿ���̨��λ
		��̨��ʼ����������
		��ͨģʽ��̨��������
		����������̨��������
		��������������
		����ǶȲ���
		
	 =============================================================================
 **/
#define PROTECT_TEMP	80
#define STANDARD 3

gimbal_t gimbal_data;
//��̨��λ
float pitch_min = 0;		
float pitch_max = 0;		


#if STANDARD == 3

		#define INFANTRY_PITCH_MAX 17.0f
		#define INFANTRY_PITCH_MIN -25.5f
    float pitch_middle = 0;
    float Pitch_min = INFANTRY_PITCH_MIN;
    float Pitch_max = INFANTRY_PITCH_MAX;

    #define VISION_PITCH_MIN            -25
    #define VISION_PITCH_MAX            17

    #define YAW_INIT_ANGLE_FDB          -yaw_Encoder.ecd_angle   //������е���������װ����yaw�������ұ������Ƕ�Ϊ���������������෴����Ҫ�Ӹ���
    #define PITCH_INIT_ANGLE_FDB        gimbal_gyro.pitch_Angle
    #define YAW_INIT_SPEED_FDB          gimbal_gyro.yaw_Gyro
    #define PITCH_INIT_SPEED_FDB        gimbal_gyro.pitch_Gyro

    #define YAW_ANGLE_FDB               gimbal_gyro.yaw_Angle
    #define PITCH_ANGLE_FDB             gimbal_gyro.pitch_Angle
    #define YAW_SPEED_FDB               gimbal_gyro.yaw_Gyro
    #define PITCH_SPEED_FDB             gimbal_gyro.pitch_Gyro

    #define VISION_YAW_ANGLE_FDB        gimbal_gyro.yaw_Angle
    #define VISION_PITCH_ANGLE_FDB      gimbal_gyro.pitch_Angle
    #define VISION_YAW_SPEED_FDB        gimbal_gyro.yaw_Gyro
    #define VISION_PITCH_SPEED_FDB      gimbal_gyro.pitch_Gyro

    #define YAW_MOTOR_POLARITY          -1
    #define PITCH_MOTOR_POLARITY        1

    float Buff_Yaw_remain = 1.3;
    float Buff_pitch_remain=-0.3;

    float auto_aim_Yaw_remain = 0;
    float auto_aim_pitch_remain = 0;
#endif




 /**
  ******************************************************************************
																��̨�ṹ���ʼ��
		pid��������
		
	 =============================================================================
 **/
void gimbal_parameter_Init(void)
{
		//�ṹ���ڴ�����
    memset(&gimbal_data, 0, sizeof(gimbal_t));
    /*******************************pid_Init*********************************/
#if STANDARD == 3
		
		PID_struct_init(&pid_chassis_angle, POSITION_PID, 600, 10, 200,0,0); 
		
    // ��ʼ���µĲ���
    PID_struct_init(&gimbal_data.pid_init_pit_Angle, POSITION_PID, 500, 4,
                    18, 0.01f, 8); //15, 0.01f, 8
    PID_struct_init(&gimbal_data.pid_init_pit_speed, POSITION_PID, 27000, 20000,
                    150, 0.001, 60); //170, 0.001f, 60
    //------------------------------------------------
    PID_struct_init(&gimbal_data.pid_init_yaw_Angle, POSITION_PID, 500, 4,
                    9.5, 0.15f, 8); 
    PID_struct_init(&gimbal_data.pid_init_yaw_speed, POSITION_PID, 29000, 10000,
                    300, 0.8f, 40); 

    // �����������µĲ���
    PID_struct_init(&gimbal_data.pid_pit_Angle, POSITION_PID, 200, 8,
                    10, 0.0f, 0); //16, 0.0f, 0
    PID_struct_init(&gimbal_data.pid_pit_speed, POSITION_PID, 25000, 2000,
                    160, 0.001f, 0); //180, 0.001f, 60
//	 PID_struct_init(&gimbal_data.pid_pit_Angle, POSITION_PID, 300, 30,
//                    20, 0.0f, 16); //15, 0.01f, 8
//    PID_struct_init(&gimbal_data.pid_pit_speed, POSITION_PID, 27000, 20000,
//                    300, 0.0, 12); //170, 0.001f, 60
    //------------------------------------------------
//		PID_struct_init(&gimbal_data.pid_yaw_Angle, POSITION_PID, 400, 16,
//                    13.8, 0.1f, 6.5);
//    PID_struct_init(&gimbal_data.pid_yaw_speed, POSITION_PID, 29000, 10000,
//                    400, 0.8f, 0); 
//	PID_struct_init(&gimbal_data.pid_yaw_Angle, POSITION_PID, 400, 16,
//                    12.8, 0.015f, 0);
//    PID_struct_init(&gimbal_data.pid_yaw_speed, POSITION_PID, 29000, 10000,
//                    420, 0.5f, 0); 
	PID_struct_init(&gimbal_data.pid_yaw_Angle, POSITION_PID, 260, 8,
                    10, 0.0f, 5);
    PID_struct_init(&gimbal_data.pid_yaw_speed, POSITION_PID, 29000, 3000,
                    300, 0.01f, 0);

    //�����²���
    PID_struct_init ( &gimbal_data.pid_pit_follow, POSITION_PID, 200, 10, 
	                  	15 , 0.01, 40 );
										
    PID_struct_init ( &gimbal_data.pid_pit_speed_follow, POSITION_PID, 27000, 25000, 
											150.0f, 0.001f, 0 ); 

    PID_struct_init ( &gimbal_data.pid_yaw_follow, POSITION_PID,  150,16,
                       6.8, 0.1, 0);//15 0 80
    PID_struct_init ( &gimbal_data.pid_yaw_speed_follow, POSITION_PID, 29800, 29800,
                       300.0f, 0.8, 0 ); //160 0.8 40
										
		//�����������²���	
    PID_struct_init(&gimbal_data.pid_rotate_pit_Angle, POSITION_PID, 200, 15,
                    17, 0.1f, 40); //15, 0.01f, 8
    PID_struct_init(&gimbal_data.pid_rotate_pit_speed, POSITION_PID, 25000, 2000,
                    180, 0.1f, 0); 
	  PID_struct_init(&gimbal_data.pid_rotate_yaw_Angle, POSITION_PID, 260, 8,
                    13.5, 0.0001f, 5);
    PID_struct_init(&gimbal_data.pid_rotate_yaw_speed, POSITION_PID, 29000, 3000,
                    480, 0.0f, 0);

    //С���µĲ���
    PID_struct_init(&gimbal_data.pid_pit_small_buff, POSITION_PID, 70, 20,
                    12.0f, 0.2f, 5); 
    PID_struct_init(&gimbal_data.pid_pit_speed_small_buff, POSITION_PID, 25000, 20000,
                    350.0f, 7.0f, 0); 
    PID_struct_init(&gimbal_data.pid_yaw_small_buff, POSITION_PID, 60, 20,
                    20.0f, 0.2f, 10);
    PID_struct_init(&gimbal_data.pid_yaw_speed_small_buff, POSITION_PID, 25000, 25000,
                    450.0f, 4.0f, 200);

    //����µĲ���
    PID_struct_init(&gimbal_data.pid_pit_big_buff, POSITION_PID, 200, 10,
                    18.0f, 0.2f, 5); 
    PID_struct_init(&gimbal_data.pid_pit_speed_big_buff, POSITION_PID, 27000, 25000,
                    150.0f, 8.0f, 0); 
    PID_struct_init(&gimbal_data.pid_yaw_big_buff, POSITION_PID, 260, 4,
                    15.0f, 0.2f, 0); 
    PID_struct_init(&gimbal_data.pid_yaw_speed_big_buff, POSITION_PID, 25000, 5000,
                    300.0f, 8.0f, 0);

#endif
    /************************************************************************/
}


 /**
  ******************************************************************************
																��̨�ܿ�������		
	 =============================================================================
 **/
void gimbal_task(void)
{
    switch (gimbal_data.ctrl_mode)
    {
    case GIMBAL_RELAX:		//�ؿ�
    {
				//�ؿ�ģʽ�£���������������㣬��ʼ����־λ����
        memset(&gimbal_data.gim_ref_and_fdb, 0, sizeof(gim_ref_and_fdb_t));
			 gimbal_data.if_finish_Init = 0;
    }
        break;
    case GIMBAL_INIT:			//��ʼ��
    {
        gimbal_init_handle();
    }
        break;
    case GIMBAL_FOLLOW_ZGYRO:		//����������
    {
        gimbal_follow_gyro_handle();
    }
        break;
    case GIMBAL_AUTO_SMALL_BUFF:	//С��
    {
        auto_small_buff_handle();
    }
        break;
    case GIMBAL_AUTO_BIG_BUFF:		//���
    {
        auto_big_buff_handle();
    }
        break;


    default:
        break;
    }
    VAL_LIMIT(gimbal_data.gim_ref_and_fdb.pit_angle_ref, pitch_min , pitch_max );		//pitch����̨�޷�
		gimbal_data.last_ctrl_mode = gimbal_data.ctrl_mode;//��̨ģʽ����

}



/****************************big_or_small_buff_var************************************/
float last_pitch_angle;
float last_yaw_angle;
float yaw_angle_ref_aim,pit_angle_ref_aim;
float last_yaw,last_pit;
uint8_t first_flag = 0;
uint8_t ved = 0;
float Delta_Dect_Angle_Yaw,Delta_Dect_Angle_Pit;
/*************************************************************************************/


 /**
  ******************************************************************************		
																��̨��ʼ������		
	 =============================================================================
 **/
void gimbal_init_handle	( void )
{
    //���������ʼ��
    first_flag = 0;
		ved = 0;
		//ָ����ʼ�������뷴��
    int init_rotate_num = 0;
    gimbal_data.gim_ref_and_fdb.pit_angle_ref = 0.0f;
    gimbal_data.gim_ref_and_fdb.pit_angle_fdb = PITCH_INIT_ANGLE_FDB;
		
    gimbal_data.gim_ref_and_fdb.yaw_angle_ref = 0.0f;
    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = YAW_INIT_ANGLE_FDB;

    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = YAW_INIT_SPEED_FDB;
    gimbal_data.gim_ref_and_fdb.pit_speed_fdb = PITCH_INIT_SPEED_FDB;
		//������̨��Ȧ������������Ȧ��������������ۼ�ֵӰ���ʼ��
    init_rotate_num = (gimbal_data.gim_ref_and_fdb.yaw_angle_fdb)/360;
    gimbal_data.gim_ref_and_fdb.yaw_angle_ref = init_rotate_num*360;

   //ͨ���ӻ�ת�����ĽǶ�
    if((gimbal_data.gim_ref_and_fdb.yaw_angle_ref-gimbal_data.gim_ref_and_fdb.yaw_angle_fdb)>=181)
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref-=360;
    else if((gimbal_data.gim_ref_and_fdb.yaw_angle_ref-gimbal_data.gim_ref_and_fdb.yaw_angle_fdb)<-179)
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref+=360;
		//pitch����yaw��˫��pid����
    gimbal_data.gim_ref_and_fdb.yaw_motor_input = pid_double_loop_cal(&gimbal_data.pid_init_yaw_Angle,
                                                                      &gimbal_data.pid_init_yaw_speed,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
																																			&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
                                                                      0 )*YAW_MOTOR_POLARITY;
    gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_init_pit_Angle,
                                                                      &gimbal_data.pid_init_pit_speed,
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
																																			&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
                                                                      0 )*PITCH_MOTOR_POLARITY;
	 //�����ж��Ƿ���ɳ�ʼ��
	if (fabs(gimbal_data.gim_ref_and_fdb.pit_angle_ref - gimbal_data.gim_ref_and_fdb.pit_angle_fdb)<=4&&fabs(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb)<=1.5)
    {
			
        gimbal_data.if_finish_Init = 1;		//��ʼ����־λ��1
                pitch_middle = PITCH_ANGLE_FDB;	//��ʼ����Ĭ��ת��ͨģʽ����ȡ��ͨģʽ�µ�pitch������ֵ������Ϊ�����ǣ�˿��Ӣ��Ϊ5015��������
			//����pitch��������λ
                pitch_max = pitch_middle+Pitch_max;
                pitch_min = pitch_middle+Pitch_min;
        
    }
    																																		
}



double convert_ecd_angle_to_0_2pi1(double ecd_angle,float _0_2pi_angle)
{
	_0_2pi_angle=fmod(YAW_POLARITY*ecd_angle*ANGLE_TO_RAD,2*PI);	
	if(_0_2pi_angle<0)
		 _0_2pi_angle+=2*PI;

	return _0_2pi_angle;
}




 /**
  ******************************************************************************
																��̨����gyro��������		
	 =============================================================================
 **/
float Auto_Shoot_Yaw_Angle_Last=0;
float K_X = 2.8f;
float K_Y = 2.8f;

float Error;
float Error_2;
float Pol;
float K_S=0;//-0.8;
float K_V=-0.2;//6
float K_3=0;
float K_yaw=0;
float yaw_angle=0;
float yaw_angle360=0;
float get_speedw=-1.1f;
void gimbal_follow_gyro_handle(void)
{
	
	 yaw_angle=convert_ecd_angle_to_0_2pi1(yaw_Encoder.ecd_angle ,yaw_angle);
		if(yaw_angle>=PI)
		{	yaw_angle360=(yaw_angle-(2*PI));}
		else
		{	yaw_angle360=yaw_angle;	}
		get_speedw = -pid_calc_filter(&pid_chassis_angle,yaw_angle360,0, 15*ANGLE_TO_RAD);
//		yaw_angle360*=RAD_TO_ANGLE;
//if(yaw_angle>=180)yaw_angle-=360;
	 
	Error_2=Error=*gimbal_data.pid_yaw_Angle.err;
	if(fabs(Error)>30)
		Error=30*fabs(Error)/Error;
	
	if(fabs(Error_2)>5)
		Error_2=5*fabs(Error_2)/Error_2;
	
	if(Error!=0)
		Pol=fabs(Error)/Error;
	else
		Pol=1;
		//����ո��л�����ģʽ����ģʽ������ʽ�����Ե�ǰ��������������ʼֵ
		if(gimbal_data.last_ctrl_mode != GIMBAL_FOLLOW_ZGYRO)
		{
			gimbal_data.gim_dynamic_ref.yaw_angle_dynamic_ref   = YAW_ANGLE_FDB;
			gimbal_data.gim_dynamic_ref.pitch_angle_dynamic_ref = PITCH_ANGLE_FDB;
		}
		//ָ����̨����

//    if(RC_CtrlData.mouse.press_r&&gimbal_data.auto_aim_rotate_flag==0&&new_location.flag==1)//����Ҽ�����  ��ƽ������
//    {

////                if (My_Auto_Shoot.Auto_Aim.Flag_Get_Target)//�Ӿ����ʶ��
//                {
//										//�л���̨����
//									/**/
////										float pitch,yaw;
////										pitch = convert_ecd_angle_to__pi_pi(VISION_PITCH_ANGLE_FDB,pitch);
////										yaw = convert_ecd_angle_to__pi_pi(VISION_YAW_ANGLE_FDB,yaw);
////										gimbal_data.gim_ref_and_fdb.pit_angle_fdb = pitch;
////                    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = yaw;
//									/**/
//									if(fabs(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb) < 2)
//									{
//										gimbal_data.if_auto_shoot = 1;
//									}else
//									{
//										gimbal_data.if_auto_shoot = 0;
//									}
//									
//                    gimbal_data.gim_ref_and_fdb.pit_angle_fdb = VISION_PITCH_ANGLE_FDB;
//                    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = VISION_YAW_ANGLE_FDB;
//                    gimbal_data.gim_ref_and_fdb.pit_speed_fdb = VISION_PITCH_SPEED_FDB;
//                    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = VISION_YAW_SPEED_FDB;
//										//�л���̨����
//                    gimbal_data.gim_ref_and_fdb.pit_angle_ref = -new_location.y + auto_aim_pitch_remain;
//                    gimbal_data.gim_ref_and_fdb.yaw_angle_ref = new_location.x + auto_aim_Yaw_remain;
//									
//									if(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb > 180.0)
//									{
//										gimbal_data.gim_ref_and_fdb.yaw_angle_ref-=360;
//									}else if(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb < -180.0)
//									{
//										gimbal_data.gim_ref_and_fdb.yaw_angle_ref+=360;
//									}
//										//�Ӿ�ģʽ����̨��λ
//                    VAL_LIMIT(gimbal_data.gim_ref_and_fdb.pit_angle_ref, VISION_PITCH_MIN , VISION_PITCH_MAX );
//                }
//				//pitch����yaw��˫��pid����
//        gimbal_data.gim_ref_and_fdb.yaw_motor_input = pid_double_loop_cal(&gimbal_data.pid_yaw_follow,
//                                                                      &gimbal_data.pid_yaw_speed_follow,
//                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
//                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
//																																			&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
//                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
//                                                                      0)*YAW_MOTOR_POLARITY;
//        gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_pit_follow,
//                                                                      &gimbal_data.pid_pit_speed_follow,
//                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
//                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
//																																			&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
//                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
//                                                                      0 )*PITCH_MOTOR_POLARITY;
//    }
//		else if((RC_CtrlData.mouse.press_r&&gimbal_data.auto_aim_rotate_flag==1&&new_location.flag==1))//С��������
		if(gimbal_data.auto_aim_rotate_flag==1&&New_Auto_Aim.if_receive_data!=0)
		{
                if (My_Auto_Shoot.Auto_Aim.Flag_Get_Target)//�Ӿ����ʶ��
                {
										//�л���̨����
									/**/
										float pitch,yaw;
										pitch = convert_ecd_angle_to__pi_pi(VISION_PITCH_ANGLE_FDB,pitch);
										yaw = convert_ecd_angle_to__pi_pi(VISION_YAW_ANGLE_FDB,yaw);
										gimbal_data.gim_ref_and_fdb.pit_angle_fdb = pitch;
                    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = yaw;
									/**/
									if(fabs(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb) < 2)
									{
										gimbal_data.if_auto_shoot = 1;
									}else
									{
										gimbal_data.if_auto_shoot = 0;
									}
//                    gimbal_data.gim_ref_and_fdb.pit_angle_fdb = VISION_PITCH_ANGLE_FDB;
//                    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = VISION_YAW_ANGLE_FDB;
                    gimbal_data.gim_ref_and_fdb.pit_speed_fdb = VISION_PITCH_SPEED_FDB;
                    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = VISION_YAW_SPEED_FDB;
										//�л���̨����
//                    gimbal_data.gim_ref_and_fdb.pit_angle_ref = -new_location.y + auto_aim_pitch_remain;
//                    gimbal_data.gim_ref_and_fdb.yaw_angle_ref = new_location.x + auto_aim_Yaw_remain;
									
										if(New_Auto_Aim.Yaw_Angle!=0&&New_Auto_Aim.Pitch_Angle!=0)
										{Auto_Shoot_Yaw_Angle_Last=New_Auto_Aim.Pitch_Angle;}
									
									  gimbal_data.gim_ref_and_fdb.pit_angle_ref = -New_Auto_Aim.Pitch_Angle + auto_aim_pitch_remain;
                    gimbal_data.gim_ref_and_fdb.yaw_angle_ref = Auto_Shoot_Yaw_Angle_Last + auto_aim_Yaw_remain;
									
									if(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb > 180.0)
									{
										gimbal_data.gim_ref_and_fdb.yaw_angle_ref-=360;
									}else if(gimbal_data.gim_ref_and_fdb.yaw_angle_ref - gimbal_data.gim_ref_and_fdb.yaw_angle_fdb < -180.0)
									{
										gimbal_data.gim_ref_and_fdb.yaw_angle_ref+=360;
									}
										//�Ӿ�ģʽ����̨��λ
                    VAL_LIMIT(gimbal_data.gim_ref_and_fdb.pit_angle_ref, VISION_PITCH_MIN , VISION_PITCH_MAX );
                }
								else
								{
				gimbal_data.gim_ref_and_fdb.pit_angle_fdb = PITCH_ANGLE_FDB;
				gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = YAW_ANGLE_FDB;
			  gimbal_data.gim_ref_and_fdb.pit_speed_fdb = PITCH_SPEED_FDB;
		    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = YAW_SPEED_FDB;
				gimbal_data.gim_ref_and_fdb.pit_angle_ref = PITCH_ANGLE_FDB;
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref = YAW_ANGLE_FDB;
								
								}
				//pitch����yaw��˫��pid����
        gimbal_data.gim_ref_and_fdb.yaw_motor_input = pid_double_loop_cal(&gimbal_data.pid_rotate_yaw_Angle,
                                                                      &gimbal_data.pid_rotate_yaw_speed,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
																																			&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
                                                                      K_V*gimbal_data.gim_ref_and_fdb.yaw_speed_fdb)*YAW_MOTOR_POLARITY;
        gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_rotate_pit_Angle,
                                                                      &gimbal_data.pid_rotate_pit_speed,
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
																																			&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
                                                                      0 )*PITCH_MOTOR_POLARITY;
		
		}
		else
    {
			
				gimbal_data.gim_ref_and_fdb.pit_angle_fdb = PITCH_ANGLE_FDB;
				gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = YAW_ANGLE_FDB;
			  gimbal_data.gim_ref_and_fdb.pit_speed_fdb = PITCH_SPEED_FDB;
		    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = YAW_SPEED_FDB;
			//��ͨģʽ����̨����
        gimbal_data.gim_ref_and_fdb.pit_angle_ref = gimbal_data.gim_dynamic_ref.pitch_angle_dynamic_ref;
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref = gimbal_data.gim_dynamic_ref.yaw_angle_dynamic_ref;
		/* yaw���� ���ȱ��� */
		if(yaw_Encoder.temperature > PROTECT_TEMP)
		{
			gimbal_data.gim_ref_and_fdb.yaw_motor_input = 0;
		}
		else
		{
			//pitch����yaw��˫��pid����
			
    gimbal_data.gim_ref_and_fdb.yaw_motor_input = K_V*gimbal_data.gim_ref_and_fdb.yaw_speed_fdb+K_3*pow(Error_2,3)+K_S*pow(Error,2)+
																																			pid_double_loop_cal(&gimbal_data.pid_yaw_Angle,
                                                                      &gimbal_data.pid_yaw_speed,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
																																			&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
                                                                      K_X*RC_CtrlData.mouse.x+K_yaw*get_speedw *Pol)*YAW_MOTOR_POLARITY;
    gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_pit_Angle,
                                                                      &gimbal_data.pid_pit_speed,
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
																																	&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
                                                                      K_Y*RC_CtrlData.mouse.y )*PITCH_MOTOR_POLARITY;
		}
    }
}








#if (STANDARD == 3)||(STANDARD == 4)||(STANDARD == 5)
 /**
  ******************************************************************************
																small_buff��������		
	 =============================================================================
 **/
void auto_small_buff_handle(void)
{
	gimbal_data.auto_aim_rotate_flag=0;
     if(first_flag == 0)
	{
		last_pitch_angle=VISION_PITCH_ANGLE_FDB;
		last_yaw_angle=VISION_YAW_ANGLE_FDB;
		first_flag = 1;
	}
    gimbal_data.gim_ref_and_fdb.pit_angle_fdb = VISION_PITCH_ANGLE_FDB;
    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = VISION_YAW_ANGLE_FDB;
    gimbal_data.gim_ref_and_fdb.pit_speed_fdb = VISION_PITCH_SPEED_FDB;
    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = VISION_YAW_SPEED_FDB;
    if(new_location.xy_0_flag)
    {
        new_location.xy_o_time++;
    }else
    {
        new_location.xy_o_time=0;
    }
    if(new_location.xy_o_time<1)
    {
        ved = 1;
        if(last_yaw==new_location.x1&&last_pit==new_location.y1)
        {
            Delta_Dect_Angle_Yaw = RAD_TO_ANGLE * atan2 ( ( (double) new_location.x1 ) * TARGET_SURFACE_LENGTH,FOCAL_LENGTH);
            Delta_Dect_Angle_Pit = RAD_TO_ANGLE * atan2 ( ( (double) new_location.y1 ) * TARGET_SURFACE_WIDTH,FOCAL_LENGTH);
				
			yaw_angle_ref_aim=Delta_Dect_Angle_Yaw + new_location.x + Buff_Yaw_remain;
			pit_angle_ref_aim=Delta_Dect_Angle_Pit + new_location.y + Buff_pitch_remain;
					if(RC_CtrlData.mouse.press_r)
			{
				gimbal_data.gim_ref_and_fdb.yaw_angle_ref = yaw_angle_ref_aim;
        gimbal_data.gim_ref_and_fdb.pit_angle_ref = raw_data_to_pitch_angle(pit_angle_ref_aim)+Buff_pitch_remain;;
			}else
			{        
				gimbal_data.gim_ref_and_fdb.pit_angle_ref = gimbal_data.gim_dynamic_ref.pitch_angle_dynamic_ref;
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref = gimbal_data.gim_dynamic_ref.yaw_angle_dynamic_ref;
			}
        }
        last_yaw=new_location.x1;
		last_pit=new_location.y1;

				

    
		
		}
    VAL_LIMIT(gimbal_data.gim_ref_and_fdb.pit_angle_ref, VISION_PITCH_MIN , VISION_PITCH_MAX );
    if(ved==0)
    {
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref = last_yaw_angle;
        gimbal_data.gim_ref_and_fdb.pit_angle_ref = last_pitch_angle;
    }
    
    gimbal_data.gim_ref_and_fdb.yaw_motor_input = pid_double_loop_cal(&gimbal_data.pid_yaw_small_buff,
                                                                      &gimbal_data.pid_yaw_speed_small_buff,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
																	&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
                                                                      0 )*YAW_MOTOR_POLARITY;
    gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_pit_small_buff,
                                                                      &gimbal_data.pid_pit_speed_small_buff,
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
																	&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
                                                                      0 )*PITCH_MOTOR_POLARITY;
}







 /**
  ******************************************************************************
																big_buff��������		
	 =============================================================================
 **/
void auto_big_buff_handle(void)
{
	gimbal_data.auto_aim_rotate_flag=0;
    if(first_flag == 0)
	{
		last_pitch_angle=VISION_PITCH_ANGLE_FDB;
		last_yaw_angle=VISION_YAW_ANGLE_FDB;
		first_flag = 1;
	}
    gimbal_data.gim_ref_and_fdb.pit_angle_fdb = VISION_PITCH_ANGLE_FDB;
    gimbal_data.gim_ref_and_fdb.yaw_angle_fdb = VISION_YAW_ANGLE_FDB;
    gimbal_data.gim_ref_and_fdb.pit_speed_fdb = VISION_PITCH_SPEED_FDB;
    gimbal_data.gim_ref_and_fdb.yaw_speed_fdb = VISION_YAW_SPEED_FDB;
    if(new_location.xy_0_flag)
    {
        new_location.xy_o_time++;
    }else
    {
        new_location.xy_o_time=0;
    }
    if(new_location.xy_o_time<1)
    {
        ved = 1;
        if(last_yaw==new_location.x1&&last_pit==new_location.y1)
        {
            Delta_Dect_Angle_Yaw = RAD_TO_ANGLE * atan2 ( ( (double) new_location.x1 ) * TARGET_SURFACE_LENGTH,FOCAL_LENGTH);
            Delta_Dect_Angle_Pit = RAD_TO_ANGLE * atan2 ( ( (double) new_location.y1 ) * TARGET_SURFACE_WIDTH,FOCAL_LENGTH);
				
			yaw_angle_ref_aim=Delta_Dect_Angle_Yaw + new_location.x + Buff_Yaw_remain;
			pit_angle_ref_aim=Delta_Dect_Angle_Pit + new_location.y + Buff_pitch_remain;

        }
        last_yaw=new_location.x1;
		    last_pit=new_location.y1;
		

				if(RC_CtrlData.mouse.press_r)
			{
				gimbal_data.gim_ref_and_fdb.yaw_angle_ref = -yaw_angle_ref_aim;
        gimbal_data.gim_ref_and_fdb.pit_angle_ref = -raw_data_to_pitch_angle(pit_angle_ref_aim)+Buff_pitch_remain;;
			}

    }
    VAL_LIMIT(gimbal_data.gim_ref_and_fdb.pit_angle_ref, VISION_PITCH_MIN , VISION_PITCH_MAX );
    if(ved==0)
    {
        gimbal_data.gim_ref_and_fdb.yaw_angle_ref = last_yaw_angle;
        gimbal_data.gim_ref_and_fdb.pit_angle_ref = last_pitch_angle;
    }
    
    gimbal_data.gim_ref_and_fdb.yaw_motor_input = pid_double_loop_cal(&gimbal_data.pid_yaw_big_buff,
                                                                      &gimbal_data.pid_yaw_speed_big_buff,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.yaw_angle_fdb,
														&gimbal_data.gim_ref_and_fdb.yaw_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.yaw_speed_fdb,
                                                                      0 )*YAW_MOTOR_POLARITY;
    gimbal_data.gim_ref_and_fdb.pitch_motor_input = pid_double_loop_cal(&gimbal_data.pid_pit_big_buff,
                                                                      &gimbal_data.pid_pit_speed_big_buff,
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_ref,                     
                                                                      gimbal_data.gim_ref_and_fdb.pit_angle_fdb,
												&gimbal_data.gim_ref_and_fdb.pit_speed_ref,
                                                                      gimbal_data.gim_ref_and_fdb.pit_speed_fdb,
                                                                      0 )*PITCH_MOTOR_POLARITY;
}








 /**
  ******************************************************************************
																��������λ�˽���	
	 =============================================================================
 **/
float raw_data_to_pitch_angle(float ecd_angle_pit)
{
  int shoot_angle_speed;
  float distance_s;
  float distance_x;
  float distance_y;
  float x1;
  float x2;
  float x3;
  float x4;
  float angle_tan;
  float shoot_radian;
  float shoot_angle;
  float real_angle;
	
  shoot_angle_speed=28;//judge_rece_mesg.shoot_data.bullet_speed;
  distance_s=6.9/cos(gimbal_gyro.pitch_Angle*ANGLE_TO_RAD);//*cos((get_yaw_angle-yaw_Angle)*ANGLE_TO_RAD));//(Gimbal_Auto_Shoot.Distance-7)/100;
	real_angle=ecd_angle_pit+RAD_TO_ANGLE * atan2 ( HEIGHT_BETWEEN_GUN_CAMERA, distance_s );
	
  distance_x=(cos((ecd_angle_pit)*ANGLE_TO_RAD)*distance_s);
  distance_y=(sin((ecd_angle_pit)*ANGLE_TO_RAD)*distance_s);

  x1=shoot_angle_speed*shoot_angle_speed;
  x2=distance_x*distance_x;
  x3=sqrt(x2-(19.6*x2*((9.8*x2)/(2*x1)+distance_y))/x1);
  x4=9.8*x2;
  angle_tan=(x1*(distance_x-x3))/(x4);
  shoot_radian=atan(angle_tan);
  shoot_angle=shoot_radian*RAD_TO_ANGLE;
  return shoot_angle;
}

#endif