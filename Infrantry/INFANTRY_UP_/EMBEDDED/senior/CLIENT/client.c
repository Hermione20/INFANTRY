#include "client.h"
u8  draw_cnt=0,draw_int=0;
u16 draw_data_ID=0x0101;
u16 data_ID=0xD180;
u16 client_custom_ID=0;
uint8_t  ddata[66];
uint8_t  tx_buf[150];
int Energy_organs_flag=0;

ext_client_custom_character_t bomb_hatch_cover;                                //���ָ�
ext_client_custom_character_t client_custom_characte_remaining_capacity;       //ʣ�൯��
ext_client_custom_character_t client_custom_character;                         //С���ݱ�־λ
ext_client_custom_character_t client_custom_character_voltage;                 //����
ext_client_custom_character_t client_custom_character_voltage_num;             //ʵʱ���ݵ���
ext_client_custom_character_t client_custom_character_Energy_organs;           //�������ص���ʱ
ext_client_custom_character_t client_custom_character_NX_time;                 //nx�������õ���ʱ
ext_client_custom_character_t client_custom_emission_frequency;                //��Ƶѡ��

ext_client_custom_graphic_seven_t   client_custom_graphic_seven;               //������ʾ
ext_client_custom_graphic_seven_t   client_custom_sight_bead;                  //׼��
ext_client_custom_graphic_seven_t   client_custom_Attack_Energy;               //���
ext_client_custom_graphic_seven_t   client_custom_Magazine_graphics;           //���ָ�ͼ��
ext_client_custom_graphic_seven_t   client_custom_character_graphics;          //С����ͼ��
ext_client_custom_graphic_seven_t   client_custom_emission_frequency_graphics; //��Ƶͼ��

#define START_POINT_X 0
#define START_POINT_Y -50
#define END_POINT_X   0
#define END_POINT_Y   50
#define RADIOS    20
#define WIDTH    2
#define OFFSET_X 1600
#define OFFSET_Y 600

typedef struct
{
  int16_t x;
  int16_t y;
} point;

point rotate_point(int16_t x,int16_t y,float angle)
{
  point result;
  float rad_angle=angle*ANGLE_TO_RAD;
  result.x=(int)(x*cos(rad_angle)-y*sin(rad_angle));
  result.y=(int)(x*sin(rad_angle)+y*cos(rad_angle));
  return result;
}


int qwe,qwer,last_qwe,qwert;
int NX_time_flag;
double NX_time,NX_time_qwe,auto_clck;

void Client_send_handle()
{
  u8 id;
  id=judge_rece_mesg.game_robot_state.robot_id;

  switch(id)
    {
    case 3:
      client_custom_ID=0x0103;
      break;
    case 4:
      client_custom_ID=0x0104;
      break;
    case 5:
      client_custom_ID=0x0105;
      break;
    case 103://��ɫ
      client_custom_ID=0x0167;
      break;
    case 104:
      client_custom_ID=0x0168;
      break;
    case 105:
      client_custom_ID=0x0169;
      break;
    }

//-----------------------------------------------------------------------------------------------------------------------------------//
//-------------------------------------------------------nxʱ�����------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------//
int i;
		auto_clck++;
		if(auto_clck<5)
		{qwert=1;}
		else {qwert=0;}
  qwe=qwert;
		
	if(last_qwe!=qwe&&qwe==1)
	{
		NX_time_flag=1;
		NX_time_qwe=50;
	}	
	NX_time=NX_time_qwe*0.03;
	
	if(NX_time==0)
	{
		NX_time_flag=0;
	}
	if(NX_time_flag==1)
	{	
		NX_time_qwe--;
	}
	if(NX_time_flag==0)
	{
		NX_time_qwe++;
	}
//-----------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------//



//************************************************************************************************************************************/
//////////////////////////////////////////////////////////////��ʼ��///////////////////////////////////////////////////////////////////
//************************************************************************************************************************************/
	switch(draw_cnt)
	{
		case 1:   //׼�Ǻ����                   1-3
    {
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id


      /*********************׼����ʾ****************************************/
      client_custom_sight_bead.grapic_data_struct[0].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_sight_bead.grapic_data_struct[0].layer=1;   //ͼ��
      client_custom_sight_bead.grapic_data_struct[0].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[2]=1;
      client_custom_sight_bead.grapic_data_struct[0].start_x=960;
      client_custom_sight_bead.grapic_data_struct[0].start_y=480;
      client_custom_sight_bead.grapic_data_struct[0].color=UI_YELLOW;
			client_custom_sight_bead.grapic_data_struct[0].radius=3;
      client_custom_sight_bead.grapic_data_struct[0].width=5;//��

//      client_custom_sight_bead.grapic_data_struct[1].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
//      client_custom_sight_bead.grapic_data_struct[1].layer=1;   //ͼ��
//      client_custom_sight_bead.grapic_data_struct[1].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[0]=0;
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[1]=0;
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[2]=2;
//      client_custom_sight_bead.grapic_data_struct[1].start_x=960;
//      client_custom_sight_bead.grapic_data_struct[1].start_y=420;
//      client_custom_sight_bead.grapic_data_struct[1].color=UI_YELLOW;
//			client_custom_sight_bead.grapic_data_struct[1].radius=3;
//      client_custom_sight_bead.grapic_data_struct[1].width=5;//��
			
		 /***********************************�������������ʾ****************************************/
      client_custom_sight_bead.grapic_data_struct[2].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_sight_bead.grapic_data_struct[2].layer=1;   //ͼ��
      client_custom_sight_bead.grapic_data_struct[2].graphic_type=1;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[1]=0;
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[2]=3;
      client_custom_sight_bead.grapic_data_struct[2].start_x=195+500;
      client_custom_sight_bead.grapic_data_struct[2].start_y=75;
      client_custom_sight_bead.grapic_data_struct[2].end_x=510+800;
      client_custom_sight_bead.grapic_data_struct[2].end_y=105;
      client_custom_sight_bead.grapic_data_struct[2].color=UI_ORANGE;
      client_custom_sight_bead.grapic_data_struct[2].width=3;
			
			/**********************************��ײ�߽�***********************************************/
			/*��߽�*/
			client_custom_sight_bead.grapic_data_struct[3].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_sight_bead.grapic_data_struct[3].layer=1;   //ͼ��
      client_custom_sight_bead.grapic_data_struct[3].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[2]=1;
      client_custom_sight_bead.grapic_data_struct[3].start_x=500;
      client_custom_sight_bead.grapic_data_struct[3].start_y=0;
      client_custom_sight_bead.grapic_data_struct[3].end_x=800;
      client_custom_sight_bead.grapic_data_struct[3].end_y=450;
      client_custom_sight_bead.grapic_data_struct[3].color=UI_WHITE;
      client_custom_sight_bead.grapic_data_struct[3].width=3;


      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_sight_bead;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_sight_bead),DN_REG_ID,tx_buf);
    }break;
		case 2:   //С����                       20
		{
				ddata[0]=0x0110;
				ddata[1]=0x0110>>8;	 //��������id
				//0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
				ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
				ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
				ddata[4]=client_custom_ID;
				ddata[5]=client_custom_ID>>8;       //�ͻ���id
					//*************************�Ƿ���С����*******************************//
				client_custom_character.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
				client_custom_character.grapic_data_struct.layer=1;   //ͼ��
				client_custom_character.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
				client_custom_character.grapic_data_struct.graphic_name[0]=0;
				client_custom_character.grapic_data_struct.graphic_name[1]=2;
				client_custom_character.grapic_data_struct.graphic_name[2]=0;

				client_custom_character.grapic_data_struct.start_x=200;
				client_custom_character.grapic_data_struct.start_y=740;
				client_custom_character.grapic_data_struct.width=WIDTH;
				client_custom_character.grapic_data_struct.start_angle=20;
				client_custom_character.grapic_data_struct.end_angle=12;
			
		if(chassis.ctrl_mode==CHASSIS_ROTATE)
			{
				client_custom_character.grapic_data_struct.color=UI_GREEN;
				client_custom_character.data[0]='R';
				client_custom_character.data[1]='O';
				client_custom_character.data[2]='T';
				client_custom_character.data[3]='A';
				client_custom_character.data[4]='T';
				client_custom_character.data[5]='E';
				client_custom_character.data[6]='_';
				client_custom_character.data[7]='O';
				client_custom_character.data[8]='P';
				client_custom_character.data[9]='E';
				client_custom_character.data[10]='N';
			}
		else
			{
				client_custom_character.grapic_data_struct.color=UI_PINK;
				client_custom_character.data[0]='R';
				client_custom_character.data[1]='O';
				client_custom_character.data[2]='T';
				client_custom_character.data[3]='A';
				client_custom_character.data[4]='T';
				client_custom_character.data[5]='E';
				client_custom_character.data[6]='_';
				client_custom_character.data[7]='O';
				client_custom_character.data[8]='F';
				client_custom_character.data[9]='F';
			}

		*(ext_client_custom_character_t*)(&ddata[6])=client_custom_character;
		data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character),DN_REG_ID,tx_buf);
		}break;
    case 3:   //��ѹ��                       4
    {
			//************************��ѹ��*******************************//
      ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_voltage.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_voltage.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_voltage.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_voltage.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage.grapic_data_struct.graphic_name[2]=4;

      client_custom_character_voltage.grapic_data_struct.start_x=200;
      client_custom_character_voltage.grapic_data_struct.start_y=670;
      client_custom_character_voltage.grapic_data_struct.width=WIDTH;
      client_custom_character_voltage.grapic_data_struct.start_angle=20;
      client_custom_character_voltage.grapic_data_struct.end_angle=8;
			client_custom_character_voltage.grapic_data_struct.color=UI_PINK;

      client_custom_character_voltage.data[0]='V';
      client_custom_character_voltage.data[1]='O';
      client_custom_character_voltage.data[2]='L';
      client_custom_character_voltage.data[3]='T';
      client_custom_character_voltage.data[4]='A';
      client_custom_character_voltage.data[5]='G';
      client_custom_character_voltage.data[6]='E';
      client_custom_character_voltage.data[7]=':';

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_voltage;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_voltage),DN_REG_ID,tx_buf);
     }break;
		case 4:   //��ѹֵ                       5
		{
			
			//************************��ѹֵ����*******************************//
		  ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_voltage_num.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_voltage_num.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_voltage_num.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[2]=5;

      client_custom_character_voltage_num.grapic_data_struct.start_x=350;
      client_custom_character_voltage_num.grapic_data_struct.start_y=670;
      client_custom_character_voltage_num.grapic_data_struct.width=WIDTH;
      client_custom_character_voltage_num.grapic_data_struct.start_angle=20;
      client_custom_character_voltage_num.grapic_data_struct.end_angle=4;
			client_custom_character_voltage_num.grapic_data_struct.color=UI_PINK;
			
      sprintf(client_custom_character_voltage_num.data,"%f",usart_down_capacitance_message.cap_voltage);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_voltage_num;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_voltage_num),DN_REG_ID,tx_buf);
    }break;
    case 5:   //��������                     6
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id

      /*************************������ʾ*******************************/
      client_custom_graphic_seven.grapic_data_struct[0].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_graphic_seven.grapic_data_struct[0].layer=1;   //ͼ��
      client_custom_graphic_seven.grapic_data_struct[0].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[2]=6;
																										 
      client_custom_graphic_seven.grapic_data_struct[0].start_x=695;
      client_custom_graphic_seven.grapic_data_struct[0].start_y=105;
      client_custom_graphic_seven.grapic_data_struct[0].end_y=105;
			
      if(usart_down_capacitance_message.cap_voltage>=0.0f&&usart_down_capacitance_message.cap_voltage<=13.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=715;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_GREEN;//UI_PINK;
        }
      else if(usart_down_capacitance_message.cap_voltage>13.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=725+((usart_down_capacitance_message.cap_voltage-13.0))*40;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_GREEN;//UI_PINK;
        }

      client_custom_graphic_seven.grapic_data_struct[0].width=30;
				
						/*�ұ߽�*/
			client_custom_graphic_seven.grapic_data_struct[1].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_graphic_seven.grapic_data_struct[1].layer=1;   //ͼ��
      client_custom_graphic_seven.grapic_data_struct[1].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[1]=2;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[2]=2;
      client_custom_graphic_seven.grapic_data_struct[1].start_x=1500;
      client_custom_graphic_seven.grapic_data_struct[1].start_y=0;
      client_custom_graphic_seven.grapic_data_struct[1].end_x=1100;
      client_custom_graphic_seven.grapic_data_struct[1].end_y=450;
      client_custom_graphic_seven.grapic_data_struct[1].color=UI_WHITE;
      client_custom_graphic_seven.grapic_data_struct[1].width=3;	
				
				
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_graphic_seven;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_graphic_seven),DN_REG_ID,tx_buf);
		}break;		
    case 6:   //��Ƶ                         7
    {
      //----------------------------------�Ƿ�Ϊ����Ƶģʽ----------------------------------------//
      ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
			client_custom_emission_frequency.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
			client_custom_emission_frequency.grapic_data_struct.layer=1;   //ͼ��
			client_custom_emission_frequency.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
			client_custom_emission_frequency.grapic_data_struct.graphic_name[0]=0;
			client_custom_emission_frequency.grapic_data_struct.graphic_name[1]=0;
			client_custom_emission_frequency.grapic_data_struct.graphic_name[2]=7;
      if(shoot.bulletspead_level==0)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='N';
          client_custom_emission_frequency.data[1]='O';
          client_custom_emission_frequency.data[2]='R';
          client_custom_emission_frequency.data[3]='M';
          client_custom_emission_frequency.data[4]='A';
          client_custom_emission_frequency.data[5]='L';
        }
      else if(shoot.bulletspead_level==1)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='M';
          client_custom_emission_frequency.data[1]='I';
          client_custom_emission_frequency.data[2]='D';
          client_custom_emission_frequency.data[3]='D';
          client_custom_emission_frequency.data[4]='L';
          client_custom_emission_frequency.data[5]='E';
        }
			else if(shoot.bulletspead_level==2)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='H';
          client_custom_emission_frequency.data[1]='I';
          client_custom_emission_frequency.data[2]='G';
          client_custom_emission_frequency.data[3]='H';
        }

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_emission_frequency;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_emission_frequency),DN_REG_ID,tx_buf);
			}break;
		case 7:   //��Ƶͼ��                     8-13
		{
				
			//----------------------------------�Ƿ�Ϊ����Ƶģʽͼ��----------------------------------------//
			ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
			client_custom_emission_frequency_graphics.grapic_data_struct[0].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_emission_frequency_graphics.grapic_data_struct[0].layer=1;   //ͼ��
      client_custom_emission_frequency_graphics.grapic_data_struct[0].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_emission_frequency_graphics.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_emission_frequency_graphics.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_emission_frequency_graphics.grapic_data_struct[0].graphic_name[2]=8;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].start_x=400;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].start_y=590;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].end_y=580;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].end_x=410;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].color=UI_PINK;
			client_custom_emission_frequency_graphics.grapic_data_struct[0].width=5;
			
			client_custom_emission_frequency_graphics.grapic_data_struct[1].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_emission_frequency_graphics.grapic_data_struct[1].layer=1;   //ͼ��
      client_custom_emission_frequency_graphics.grapic_data_struct[1].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_emission_frequency_graphics.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_emission_frequency_graphics.grapic_data_struct[1].graphic_name[1]=0;
      client_custom_emission_frequency_graphics.grapic_data_struct[1].graphic_name[2]=9;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].start_x=400;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].start_y=570;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].end_y=580;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].end_x=410;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].color=UI_PINK;
			client_custom_emission_frequency_graphics.grapic_data_struct[1].width=5;
			
			if(shoot.bulletspead_level==1)
			{
				client_custom_emission_frequency_graphics.grapic_data_struct[2].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[2].layer=1;   //ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
				client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[0]=0;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[1]=1;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[2]=0;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].start_x=420;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].start_y=590;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].end_y=580;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].end_x=430;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].color=UI_PINK;
				client_custom_emission_frequency_graphics.grapic_data_struct[2].width=5;
				
				client_custom_emission_frequency_graphics.grapic_data_struct[3].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[3].layer=1;   //ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
				client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[0]=0;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[1]=1;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[2]=1;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].start_x=420;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].start_y=570;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].end_y=580;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].end_x=430;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].color=UI_PINK;
				client_custom_emission_frequency_graphics.grapic_data_struct[3].width=5;
		  }
			
			if(shoot.bulletspead_level==2)
			{
				client_custom_emission_frequency_graphics.grapic_data_struct[4].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[4].layer=1;   //ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
				client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[0]=0;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[1]=1;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[2]=2;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].start_x=440;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].start_y=590;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].end_y=580;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].end_x=450;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].color=UI_PINK;
				client_custom_emission_frequency_graphics.grapic_data_struct[4].width=5;
				
				client_custom_emission_frequency_graphics.grapic_data_struct[5].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[5].layer=1;   //ͼ��
				client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
				client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[0]=0;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[1]=1;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[2]=3;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].start_x=440;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].start_y=570;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].end_y=580;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].end_x=450;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].color=UI_PINK;
				client_custom_emission_frequency_graphics.grapic_data_struct[5].width=5;
		  }
			
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_emission_frequency_graphics;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_emission_frequency_graphics),DN_REG_ID,tx_buf);
    }break;
		case 10:  //�Ƿ��⵽������������       17
		{
			//************************************����**********************************************//
			ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_Energy_organs.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_Energy_organs.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_Energy_organs.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[1]=1;
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[2]=7;

      client_custom_character_Energy_organs.grapic_data_struct.start_x=240;
      client_custom_character_Energy_organs.grapic_data_struct.start_y=710;
      client_custom_character_Energy_organs.grapic_data_struct.width=WIDTH;
      client_custom_character_Energy_organs.grapic_data_struct.start_angle=15;
      client_custom_character_Energy_organs.grapic_data_struct.end_angle=8;

			if(qwe==1)
				{
					client_custom_character_Energy_organs.grapic_data_struct.color=UI_GREEN;
					client_custom_character_Energy_organs.data[0]='D';
					client_custom_character_Energy_organs.data[1]='E';
					client_custom_character_Energy_organs.data[2]='T';
					client_custom_character_Energy_organs.data[3]='E';
					client_custom_character_Energy_organs.data[4]='C';
					client_custom_character_Energy_organs.data[5]='T';
				}
			else
				{
					client_custom_character_Energy_organs.grapic_data_struct.color=UI_PINK;
					client_custom_character_Energy_organs.data[0]='U';
					client_custom_character_Energy_organs.data[1]='N';
					client_custom_character_Energy_organs.data[2]='D';
					client_custom_character_Energy_organs.data[3]='E';
					client_custom_character_Energy_organs.data[4]='T';
					client_custom_character_Energy_organs.data[5]='E';
					client_custom_character_Energy_organs.data[6]='C';
					client_custom_character_Energy_organs.data[7]='T';
				}

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_Energy_organs;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_Energy_organs),DN_REG_ID,tx_buf);
		}break;
		case 11:  //nxʱ������                   18
		{
			ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_NX_time.grapic_data_struct.operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_NX_time.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_NX_time.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_NX_time.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_NX_time.grapic_data_struct.graphic_name[1]=1;
      client_custom_character_NX_time.grapic_data_struct.graphic_name[2]=8;

      client_custom_character_NX_time.grapic_data_struct.start_x=410;
      client_custom_character_NX_time.grapic_data_struct.start_y=710;
      client_custom_character_NX_time.grapic_data_struct.width=WIDTH;
      client_custom_character_NX_time.grapic_data_struct.start_angle=20;
      client_custom_character_NX_time.grapic_data_struct.end_angle=4;
			client_custom_character_NX_time.grapic_data_struct.color=UI_PINK;
				
			sprintf(client_custom_character_NX_time.data,"%f",NX_time);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_NX_time;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_NX_time),DN_REG_ID,tx_buf);	
		}break;
		case 12:  //�Ƿ��⵽��������ͼ��       19
		{
			ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id		

			client_custom_Attack_Energy.grapic_data_struct[0].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
			client_custom_Attack_Energy.grapic_data_struct[0].layer=1;   //ͼ��
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[0]=0;
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[1]=1;
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[2]=9;
			if(qwe==1)
				{
				client_custom_Attack_Energy.grapic_data_struct[0].start_x=220;
				client_custom_Attack_Energy.grapic_data_struct[0].start_y=697;
				client_custom_Attack_Energy.grapic_data_struct[0].radius=3;
				client_custom_Attack_Energy.grapic_data_struct[0].color=UI_GREEN;
				client_custom_Attack_Energy.grapic_data_struct[0].width=9;
				}
			else
				{
				client_custom_Attack_Energy.grapic_data_struct[0].start_x=220;
				client_custom_Attack_Energy.grapic_data_struct[0].start_y=697;
				client_custom_Attack_Energy.grapic_data_struct[0].radius=3;
				client_custom_Attack_Energy.grapic_data_struct[0].color=UI_PINK;
				client_custom_Attack_Energy.grapic_data_struct[0].width=9;
				}
			
			*(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_Attack_Energy;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_Attack_Energy),DN_REG_ID,tx_buf);
		}
	
	/*******************************************************************************************************************************/
	/////////////////////////////////////////////////////////ˢ��ѭ��////////////////////////////////////////////////////////////////
	/*******************************************************************************************************************************/
		case 13:  //С����                       20
		{
					ddata[0]=0x0110;
          ddata[1]=0x0110>>8;	 //��������id
          //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
          ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
          ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
          ddata[4]=client_custom_ID;
          ddata[5]=client_custom_ID>>8;       //�ͻ���id
			      //*************************�Ƿ���С����*******************************//
					client_custom_character.grapic_data_struct.operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
          client_custom_character.grapic_data_struct.layer=1;   //ͼ��
          client_custom_character.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
          client_custom_character.grapic_data_struct.graphic_name[0]=0;
          client_custom_character.grapic_data_struct.graphic_name[1]=2;
          client_custom_character.grapic_data_struct.graphic_name[2]=0;

          client_custom_character.grapic_data_struct.start_x=200;
          client_custom_character.grapic_data_struct.start_y=740;
          client_custom_character.grapic_data_struct.width=WIDTH;
          client_custom_character.grapic_data_struct.start_angle=20;
          client_custom_character.grapic_data_struct.end_angle=12;
      if(chassis.ctrl_mode==CHASSIS_ROTATE)
        {
          client_custom_character.grapic_data_struct.color=UI_GREEN;
          client_custom_character.data[0]='R';
          client_custom_character.data[1]='O';
          client_custom_character.data[2]='T';
          client_custom_character.data[3]='A';
          client_custom_character.data[4]='T';
          client_custom_character.data[5]='E';
          client_custom_character.data[6]='_';
          client_custom_character.data[7]='O';
          client_custom_character.data[8]='P';
          client_custom_character.data[9]='E';
          client_custom_character.data[10]='N';
        }
      else
        {
          client_custom_character.grapic_data_struct.color=UI_PINK;
          client_custom_character.data[0]='R';
          client_custom_character.data[1]='O';
          client_custom_character.data[2]='T';
          client_custom_character.data[3]='A';
          client_custom_character.data[4]='T';
          client_custom_character.data[5]='E';
          client_custom_character.data[6]='_';
          client_custom_character.data[7]='O';
          client_custom_character.data[8]='F';
          client_custom_character.data[9]='F';
					client_custom_character.data[10]='\n';
        }

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character),DN_REG_ID,tx_buf);
		}break;
		case 14:  //��ѹ����ֵ                   5
		{
			//-------------------------------------------��ѹ����ֵ---------------------------------------------//
			ddata[0]=(uint8_t)0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id

      client_custom_character_voltage_num.grapic_data_struct.operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_voltage_num.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_voltage_num.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[2]=5;

      client_custom_character_voltage_num.grapic_data_struct.start_x=350;
      client_custom_character_voltage_num.grapic_data_struct.start_y=670;
      client_custom_character_voltage_num.grapic_data_struct.width=WIDTH;
      client_custom_character_voltage_num.grapic_data_struct.start_angle=20;
      client_custom_character_voltage_num.grapic_data_struct.end_angle=4;
      sprintf(client_custom_character_voltage_num.data,"%f",usart_down_capacitance_message.cap_voltage);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_voltage_num;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_voltage_num),DN_REG_ID,tx_buf);
		}break;
		case 15:  //��������                     6
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id

      client_custom_graphic_seven.grapic_data_struct[0].operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_graphic_seven.grapic_data_struct[0].layer=1;   //ͼ��
      client_custom_graphic_seven.grapic_data_struct[0].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[2]=6;
																										 
      client_custom_graphic_seven.grapic_data_struct[0].start_x=695;
      client_custom_graphic_seven.grapic_data_struct[0].start_y=90;
      client_custom_graphic_seven.grapic_data_struct[0].end_y=90;
			
      if(usart_down_capacitance_message.cap_voltage>=0.0f&&usart_down_capacitance_message.cap_voltage<=16.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=715;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_RB;//UI_PINK;
        }
      else if(usart_down_capacitance_message.cap_voltage>16.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=735+((usart_down_capacitance_message.cap_voltage-13.0))*50;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_YELLOW;//UI_PINK;
        }

      client_custom_graphic_seven.grapic_data_struct[0].width=30;
				
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_graphic_seven;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_graphic_seven),DN_REG_ID,tx_buf);
		}break;
 		case 16:  //��Ƶ                         7
		{
			//----------------------------------�Ƿ�Ϊ����Ƶģʽ----------------------------------------//
      ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
			client_custom_emission_frequency.grapic_data_struct.operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
			client_custom_emission_frequency.grapic_data_struct.layer=1;   //ͼ��
			client_custom_emission_frequency.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
			client_custom_emission_frequency.grapic_data_struct.graphic_name[0]=0;
			client_custom_emission_frequency.grapic_data_struct.graphic_name[1]=0;
			client_custom_emission_frequency.grapic_data_struct.graphic_name[2]=7;
      if(shoot.bulletspead_level==0)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='N';
          client_custom_emission_frequency.data[1]='O';
          client_custom_emission_frequency.data[2]='R';
          client_custom_emission_frequency.data[3]='M';
          client_custom_emission_frequency.data[4]='A';
          client_custom_emission_frequency.data[5]='L';
        }
      else if(shoot.bulletspead_level==1)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='M';
          client_custom_emission_frequency.data[1]='I';
          client_custom_emission_frequency.data[2]='D';
          client_custom_emission_frequency.data[3]='D';
          client_custom_emission_frequency.data[4]='L';
          client_custom_emission_frequency.data[5]='E';
        }
			else if(shoot.bulletspead_level==2)
        {
          client_custom_emission_frequency.grapic_data_struct.start_x=200;
          client_custom_emission_frequency.grapic_data_struct.start_y=595;
          client_custom_emission_frequency.grapic_data_struct.width=WIDTH;
          client_custom_emission_frequency.grapic_data_struct.start_angle=20;
          client_custom_emission_frequency.grapic_data_struct.end_angle=11;
          client_custom_emission_frequency.grapic_data_struct.color=UI_PINK;
          client_custom_emission_frequency.data[0]='H';
          client_custom_emission_frequency.data[1]='I';
          client_custom_emission_frequency.data[2]='G';
          client_custom_emission_frequency.data[3]='H';
        }

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_emission_frequency;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_emission_frequency),DN_REG_ID,tx_buf);
			}break;
		case 17:  //��Ƶͼ��                     10-13
		{
						//----------------------------------�Ƿ�Ϊ����Ƶģʽͼ��----------------------------------------//
			ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
						
			if(shoot.bulletspead_level==1)
				{
					client_custom_emission_frequency_graphics.grapic_data_struct[2].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[2].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[2]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].start_x=420;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].start_y=590;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].end_y=580;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].end_x=430;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].color=UI_PINK;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].width=5;
					
					client_custom_emission_frequency_graphics.grapic_data_struct[3].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[3].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[2]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].start_x=420;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].start_y=570;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].end_y=580;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].end_x=430;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].color=UI_PINK;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].width=5;
				}
			else
				{
					client_custom_emission_frequency_graphics.grapic_data_struct[2].operate_type=3;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[2].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[2].graphic_name[2]=0;
					
					client_custom_emission_frequency_graphics.grapic_data_struct[3].operate_type=3;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[3].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[3].graphic_name[2]=1;
				}
			if(shoot.bulletspead_level==2)
				{
					client_custom_emission_frequency_graphics.grapic_data_struct[4].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[4].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[2]=2;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].start_x=440;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].start_y=590;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].end_y=580;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].end_x=450;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].color=UI_PINK;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].width=5;
					
					client_custom_emission_frequency_graphics.grapic_data_struct[5].operate_type=1;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[5].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[2]=3;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].start_x=440;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].start_y=570;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].end_y=580;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].end_x=450;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].color=UI_PINK;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].width=5;
				}
			else
				{
					client_custom_emission_frequency_graphics.grapic_data_struct[4].operate_type=3;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[4].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[4].graphic_name[2]=2;
					
					client_custom_emission_frequency_graphics.grapic_data_struct[5].operate_type=3;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[5].layer=1;   //ͼ��
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_type=0;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[0]=0;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[1]=1;
					client_custom_emission_frequency_graphics.grapic_data_struct[5].graphic_name[2]=3;
				}
			
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_emission_frequency_graphics;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_emission_frequency_graphics),DN_REG_ID,tx_buf);	
		}break;
		case 20:  //�Ƿ��⵽������������       17         
		{
			//************************************����**********************************************//
			ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_Energy_organs.grapic_data_struct.operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_Energy_organs.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_Energy_organs.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[1]=1;
      client_custom_character_Energy_organs.grapic_data_struct.graphic_name[2]=7;

      client_custom_character_Energy_organs.grapic_data_struct.start_x=240;
      client_custom_character_Energy_organs.grapic_data_struct.start_y=710;
      client_custom_character_Energy_organs.grapic_data_struct.width=WIDTH;
      client_custom_character_Energy_organs.grapic_data_struct.start_angle=15;
      client_custom_character_Energy_organs.grapic_data_struct.end_angle=8;

			if(qwe==1)
				{
					client_custom_character_Energy_organs.grapic_data_struct.color=UI_GREEN;
					client_custom_character_Energy_organs.data[0]='D';
					client_custom_character_Energy_organs.data[1]='E';
					client_custom_character_Energy_organs.data[2]='T';
					client_custom_character_Energy_organs.data[3]='E';
					client_custom_character_Energy_organs.data[4]='C';
					client_custom_character_Energy_organs.data[5]='T';
				}
			else
				{
					client_custom_character_Energy_organs.grapic_data_struct.color=UI_PINK;
					client_custom_character_Energy_organs.data[0]='U';
					client_custom_character_Energy_organs.data[1]='N';
					client_custom_character_Energy_organs.data[2]='D';
					client_custom_character_Energy_organs.data[3]='E';
					client_custom_character_Energy_organs.data[4]='T';
					client_custom_character_Energy_organs.data[5]='E';
					client_custom_character_Energy_organs.data[6]='C';
					client_custom_character_Energy_organs.data[7]='T';
				}

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_Energy_organs;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_Energy_organs),DN_REG_ID,tx_buf);
		}break;
		case 21:  //�Ƿ��⵽��������ͼ��       19
		{
			ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id		

			client_custom_Attack_Energy.grapic_data_struct[0].operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
			client_custom_Attack_Energy.grapic_data_struct[0].layer=1;   //ͼ��
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[0]=0;
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[1]=1;
			client_custom_Attack_Energy.grapic_data_struct[0].graphic_name[2]=9;
			if(qwe==1)
				{
				client_custom_Attack_Energy.grapic_data_struct[0].start_x=220;
				client_custom_Attack_Energy.grapic_data_struct[0].start_y=697;
				client_custom_Attack_Energy.grapic_data_struct[0].radius=3;
				client_custom_Attack_Energy.grapic_data_struct[0].color=UI_GREEN;
				client_custom_Attack_Energy.grapic_data_struct[0].width=9;
				}
			else
				{
				client_custom_Attack_Energy.grapic_data_struct[0].start_x=220;
				client_custom_Attack_Energy.grapic_data_struct[0].start_y=697;
				client_custom_Attack_Energy.grapic_data_struct[0].radius=3;
				client_custom_Attack_Energy.grapic_data_struct[0].color=UI_PINK;
				client_custom_Attack_Energy.grapic_data_struct[0].width=9;
				}
			
			*(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_Attack_Energy;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_Attack_Energy),DN_REG_ID,tx_buf);
		}break;
		case 22:  //nxʱ������                   18
		{
			ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id
			
      client_custom_character_NX_time.grapic_data_struct.operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_character_NX_time.grapic_data_struct.layer=1;   //ͼ��
      client_custom_character_NX_time.grapic_data_struct.graphic_type=7;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_character_NX_time.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_NX_time.grapic_data_struct.graphic_name[1]=1;
      client_custom_character_NX_time.grapic_data_struct.graphic_name[2]=8;

      client_custom_character_NX_time.grapic_data_struct.start_x=410;
      client_custom_character_NX_time.grapic_data_struct.start_y=710;
      client_custom_character_NX_time.grapic_data_struct.width=WIDTH;
      client_custom_character_NX_time.grapic_data_struct.start_angle=20;
      client_custom_character_NX_time.grapic_data_struct.end_angle=4;
			client_custom_character_NX_time.grapic_data_struct.color=UI_PINK;
				
			sprintf(client_custom_character_NX_time.data,"%f",NX_time);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_NX_time;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_NX_time),DN_REG_ID,tx_buf);	

		}break;
		case 23:
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //��������id
      //0x0100  ɾ��ͼ�� 0x0101 ����һ��ͼ�� 0x0102 ���ƶ���ͼ�� 0x0103 �������ͼ�� 0x0104�����߸�ͼ�� 0x0110�ͻ��˻����ַ�ͼ��
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //������id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //�ͻ���id


      /*********************׼����ʾ****************************************/
      client_custom_sight_bead.grapic_data_struct[0].operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
      client_custom_sight_bead.grapic_data_struct[0].layer=1;   //ͼ��
      client_custom_sight_bead.grapic_data_struct[0].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_sight_bead.grapic_data_struct[0].graphic_name[2]=1;
      client_custom_sight_bead.grapic_data_struct[0].start_x=960;
      client_custom_sight_bead.grapic_data_struct[0].start_y=480;
//      client_custom_sight_bead.grapic_data_struct[0].color=UI_YELLOW;
			client_custom_sight_bead.grapic_data_struct[0].radius=3;
      client_custom_sight_bead.grapic_data_struct[0].width=5;//��

//      client_custom_sight_bead.grapic_data_struct[1].operate_type=2;  //1 ���� 2�޸�ͼ�� 3ɾ������ͼ�� 5ɾ��һ��ͼ���ͼ�� 6ɾ������ͼ��
//      client_custom_sight_bead.grapic_data_struct[1].layer=1;   //ͼ��
//      client_custom_sight_bead.grapic_data_struct[1].graphic_type=2;  //0 ֱ�� 1���� 2��Բ 3��Բ 4Բ�� 5������ 6������ 7�ַ�
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[0]=0;
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[1]=0;
//      client_custom_sight_bead.grapic_data_struct[1].graphic_name[2]=2;
//      client_custom_sight_bead.grapic_data_struct[1].start_x=960;
//      client_custom_sight_bead.grapic_data_struct[1].start_y=420;
////      client_custom_sight_bead.grapic_data_struct[1].color=UI_YELLOW;
//			client_custom_sight_bead.grapic_data_struct[1].radius=3;
//      client_custom_sight_bead.grapic_data_struct[1].width=5;//��
			if(shoot.fric_wheel_run==1)
			{
			client_custom_sight_bead.grapic_data_struct[0].color=UI_YELLOW;
//			client_custom_sight_bead.grapic_data_struct[1].color=UI_YELLOW;
			}
			else 
			{			
			client_custom_sight_bead.grapic_data_struct[0].color=UI_RB;
//			client_custom_sight_bead.grapic_data_struct[1].color=UI_RB;
			}
			*(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_sight_bead;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_sight_bead),DN_REG_ID,tx_buf);
		}break;
    default:
    break;
    }

  draw_cnt++;
	draw_int++;
  if(draw_cnt>23)//����Ҫˢ�µ�ͼ��ˢ��
    draw_cnt=13;
	
	last_qwe=qwe;
}

void delete_Coverage(u8 coverage)
{
  ddata[6]=4;//1����2�޸�3ɾ������4ɾ��ͼ��5ɾ������
  ddata[13]=coverage;//ͼ��0-9
}