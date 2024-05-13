#include "client.h"
u8  draw_cnt=0,draw_int=0;
u16 draw_data_ID=0x0101;
u16 data_ID=0xD180;
u16 client_custom_ID=0;
uint8_t  ddata[66];
uint8_t  tx_buf[150];
int Energy_organs_flag=0;

ext_client_custom_character_t bomb_hatch_cover;                                //弹仓盖
ext_client_custom_character_t client_custom_bullet_hatch_cover;       //剩余弹量
ext_client_custom_character_t client_custom_character;                         //小陀螺标志位
ext_client_custom_character_t client_custom_character_voltage;                 //电容
ext_client_custom_character_t client_custom_character_voltage_num;             //实时电容电量

ext_client_custom_graphic_seven_t   client_custom_graphic_seven;               //电量显示
ext_client_custom_graphic_seven_t   client_custom_sight_bead;                  //准星
ext_client_custom_graphic_seven_t   client_custom_Attack_Energy;               //打符
ext_client_custom_graphic_seven_t   client_custom_bomb_hatch_cover;           //弹仓盖图形
ext_client_custom_graphic_seven_t   client_custom_character_graphics;          //小陀螺图形
ext_client_custom_graphic_seven_t   client_custom_emission_frequency_graphics; //射频图案

#define START_POINT_X 0
#define START_POINT_Y -50
#define END_POINT_X   0
#define END_POINT_Y   50
#define RADIOS    20
#define WIDTH    4
#define OFFSET_X 1600
#define OFFSET_Y 600

uint32_t R_Board =1;
uint32_t Board_Included_Angle =PI/6;
uint32_t Center_of_Circle_X =960;
uint32_t Center_of_Circle_Y =540;
uint32_t OFFSET =0;




//建议范围 x（960+-120*2.75） y（540+-280）
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
    case 103://蓝色
      client_custom_ID=0x0167;
      break;
    case 104:
      client_custom_ID=0x0168;
      break;
    case 105:
      client_custom_ID=0x0169;
      break;
    }

//************************************************************************************************************************************/
//////////////////////////////////////////////////////////////初始化///////////////////////////////////////////////////////////////////
//************************************************************************************************************************************/
	switch(draw_cnt)
	{
		case 1:   //静态显示
    {
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id
		 /***********************************超级电容外框显示****************************************/
      client_custom_sight_bead.grapic_data_struct[1].operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_sight_bead.grapic_data_struct[1].layer=2;   //图层
      client_custom_sight_bead.grapic_data_struct[1].graphic_type=1;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_sight_bead.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[1].graphic_name[1]=0;
      client_custom_sight_bead.grapic_data_struct[1].graphic_name[2]=3;
      client_custom_sight_bead.grapic_data_struct[1].start_x=195+500;
      client_custom_sight_bead.grapic_data_struct[1].start_y=75;
      client_custom_sight_bead.grapic_data_struct[1].end_x=510+800;
      client_custom_sight_bead.grapic_data_struct[1].end_y=105;
      client_custom_sight_bead.grapic_data_struct[1].color=UI_ORANGE;
      client_custom_sight_bead.grapic_data_struct[1].width=8;
			/**********************************碰撞边界***********************************************/
			/*左边界*/
			client_custom_sight_bead.grapic_data_struct[2].operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_sight_bead.grapic_data_struct[2].layer=1;   //图层
      client_custom_sight_bead.grapic_data_struct[2].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[1]=2;
      client_custom_sight_bead.grapic_data_struct[2].graphic_name[2]=1;
      client_custom_sight_bead.grapic_data_struct[2].start_x=500;
      client_custom_sight_bead.grapic_data_struct[2].start_y=0;
      client_custom_sight_bead.grapic_data_struct[2].end_x=800;
      client_custom_sight_bead.grapic_data_struct[2].end_y=450;
      client_custom_sight_bead.grapic_data_struct[2].color=UI_WHITE;
      client_custom_sight_bead.grapic_data_struct[2].width=3;
		  /*右边界*/
			client_custom_sight_bead.grapic_data_struct[3].operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_sight_bead.grapic_data_struct[3].layer=1;   //图层
      client_custom_sight_bead.grapic_data_struct[3].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_sight_bead.grapic_data_struct[3].graphic_name[2]=2;
      client_custom_sight_bead.grapic_data_struct[3].start_x=1500;
      client_custom_sight_bead.grapic_data_struct[3].start_y=0;
      client_custom_sight_bead.grapic_data_struct[3].end_x=1100;
      client_custom_sight_bead.grapic_data_struct[3].end_y=450;
      client_custom_sight_bead.grapic_data_struct[3].color=UI_WHITE;
      client_custom_sight_bead.grapic_data_struct[3].width=3;	

      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_sight_bead;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_sight_bead),DN_REG_ID,tx_buf);
			

    }break;
		case 2:   //小陀螺                       20
		{
				ddata[0]=0x0110;
				ddata[1]=0x0110>>8;	 //数据内容id
				//0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
				ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
				ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
				ddata[4]=client_custom_ID;
				ddata[5]=client_custom_ID>>8;       //客户端id
					//*************************是否开启小陀螺*******************************//
				client_custom_character.grapic_data_struct.operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
				client_custom_character.grapic_data_struct.layer=1;   //图层
				client_custom_character.grapic_data_struct.graphic_type=7;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
				client_custom_character.grapic_data_struct.graphic_name[0]=0;
				client_custom_character.grapic_data_struct.graphic_name[1]=2;
				client_custom_character.grapic_data_struct.graphic_name[2]=0;

				client_custom_character.grapic_data_struct.start_x=200;
				client_custom_character.grapic_data_struct.start_y=740;
				client_custom_character.grapic_data_struct.width=WIDTH;
				client_custom_character.grapic_data_struct.start_angle=20;
				client_custom_character.grapic_data_struct.end_angle=12;
			
				client_custom_character.grapic_data_struct.color=UI_GREEN;
				client_custom_character.data[0] ='R';
				client_custom_character.data[1] ='O';
				client_custom_character.data[2] ='T';
				client_custom_character.data[3] ='A';
				client_custom_character.data[4] ='T';
				client_custom_character.data[5] ='E';
				client_custom_character.data[6] =':';

		*(ext_client_custom_character_t*)(&ddata[6])=client_custom_character;
		data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character),DN_REG_ID,tx_buf);
		}break;
    case 3:   //电压字                       4
    {
			//************************电压字*******************************//
      ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id
			
      client_custom_character_voltage.grapic_data_struct.operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_character_voltage.grapic_data_struct.layer=1;   //图层
      client_custom_character_voltage.grapic_data_struct.graphic_type=7;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_character_voltage.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage.grapic_data_struct.graphic_name[2]=4;

      client_custom_character_voltage.grapic_data_struct.start_x=195+500;
      client_custom_character_voltage.grapic_data_struct.start_y=75;
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
		case 4:   //电压值                       5//（更新）
		{
			
			//************************电压值数字*******************************//
		  ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id
			
      client_custom_character_voltage_num.grapic_data_struct.operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_character_voltage_num.grapic_data_struct.layer=1;   //图层
      client_custom_character_voltage_num.grapic_data_struct.graphic_type=7;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[2]=5;

      client_custom_character_voltage_num.grapic_data_struct.start_x=195+500;
      client_custom_character_voltage_num.grapic_data_struct.start_y=75;
      client_custom_character_voltage_num.grapic_data_struct.width=WIDTH;
      client_custom_character_voltage_num.grapic_data_struct.start_angle=20;
      client_custom_character_voltage_num.grapic_data_struct.end_angle=4;
			client_custom_character_voltage_num.grapic_data_struct.color=UI_PINK;
			
      sprintf(client_custom_character_voltage_num.data,"%f",usart_down_capacitance_message.cap_voltage);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_voltage_num;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_voltage_num),DN_REG_ID,tx_buf);
    }break;
		case 5:
    {
			//************************电压字*******************************//
      ddata[0]=0x0110;
      ddata[1]=0x0110>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id
			
      bomb_hatch_cover.grapic_data_struct.operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      bomb_hatch_cover.grapic_data_struct.layer=1;   //图层
      bomb_hatch_cover.grapic_data_struct.graphic_type=7;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      bomb_hatch_cover.grapic_data_struct.graphic_name[0]=0;
      bomb_hatch_cover.grapic_data_struct.graphic_name[1]=3;
      bomb_hatch_cover.grapic_data_struct.graphic_name[2]=0;

      bomb_hatch_cover.grapic_data_struct.start_x=200;
      bomb_hatch_cover.grapic_data_struct.start_y=640;
      bomb_hatch_cover.grapic_data_struct.width=WIDTH;
      bomb_hatch_cover.grapic_data_struct.start_angle=20;
      bomb_hatch_cover.grapic_data_struct.end_angle=8;
			bomb_hatch_cover.grapic_data_struct.color=UI_GREEN;

      bomb_hatch_cover.data[0]='D';
      bomb_hatch_cover.data[1]='O';
      bomb_hatch_cover.data[2]='O';
      bomb_hatch_cover.data[3]='R';
      bomb_hatch_cover.data[4]=':';

      *(ext_client_custom_character_t*)(&ddata[6])=bomb_hatch_cover;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(bomb_hatch_cover),DN_REG_ID,tx_buf);
     }break;
    case 6:   //电量长条  更新
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id

      /*************************电量显示*******************************/
			
      client_custom_graphic_seven.grapic_data_struct[0].operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_graphic_seven.grapic_data_struct[0].layer=1;   //图层
      client_custom_graphic_seven.grapic_data_struct[0].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[2]=6;
																										 
      client_custom_graphic_seven.grapic_data_struct[0].start_x=695;
      client_custom_graphic_seven.grapic_data_struct[0].start_y=90;
      client_custom_graphic_seven.grapic_data_struct[0].end_y=90;
			
      if(usart_down_capacitance_message.cap_voltage>=0.0f&&usart_down_capacitance_message.cap_voltage<=5.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=715;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_RB;//UI_PINK;
        }
      else if(usart_down_capacitance_message.cap_voltage>5.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=735+((usart_down_capacitance_message.cap_voltage-5.0f)*725/24);
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_YELLOW;//UI_PINK;
        }

      client_custom_graphic_seven.grapic_data_struct[0].width=30;
				
			
      /*********************准星显示****************************************/
      client_custom_graphic_seven.grapic_data_struct[1].operate_type=1;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_graphic_seven.grapic_data_struct[1].layer=1;   //图层
      client_custom_graphic_seven.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[2]=1;
      client_custom_graphic_seven.grapic_data_struct[1].start_x=960;
      client_custom_graphic_seven.grapic_data_struct[1].start_y=480;
      client_custom_graphic_seven.grapic_data_struct[1].color=UI_RB;
			client_custom_graphic_seven.grapic_data_struct[1].radius=3;
      client_custom_graphic_seven.grapic_data_struct[1].width=5;//竖

//			/*********************装甲板ID 1****************************************/	
//				
//			client_custom_graphic_seven.grapic_data_struct[2].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[2].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[2].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[2]=7;
//      client_custom_graphic_seven.grapic_data_struct[2].start_x=(u32)Center_of_Circle_X+cosf(yaw_angle360)*R_Board;
//      client_custom_graphic_seven.grapic_data_struct[2].start_y=(u32)R_Board*sinf(yaw_angle360)+Center_of_Circle_Y+OFFSET;
//			client_custom_graphic_seven.grapic_data_struct[2].end_x=(u32)(-R_Board*cosf(yaw_angle360)+Center_of_Circle_X);
//			client_custom_graphic_seven.grapic_data_struct[2].end_y=(u32)(-R_Board*sinf(yaw_angle360)+Center_of_Circle_Y+OFFSET);
//      client_custom_graphic_seven.grapic_data_struct[2].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[2].width=6;//竖	
//			
//			/*********************装甲板ID 2****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[3].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[3].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[3].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[2]=8;
//      client_custom_graphic_seven.grapic_data_struct[3].start_x=(u32)R_Board*cosf(yaw_angle360+PI/2)+Center_of_Circle_X-OFFSET;
//      client_custom_graphic_seven.grapic_data_struct[3].start_y=(u32)R_Board*sinf(yaw_angle360+PI/2)+Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[3].end_x=(u32)(-R_Board*cosf(yaw_angle360+PI/2)+Center_of_Circle_X-OFFSET);
//			client_custom_graphic_seven.grapic_data_struct[3].end_y=(u32)(-R_Board*sinf(yaw_angle360+PI/2)+Center_of_Circle_Y);
//      client_custom_graphic_seven.grapic_data_struct[3].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[3].width=6;//竖	
//			
//			/*********************装甲板ID 3****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[4].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[4].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[4].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[2]=9;
//      client_custom_graphic_seven.grapic_data_struct[4].start_x=(u32)R_Board*cosf(yaw_angle360+PI)+Center_of_Circle_X;
//      client_custom_graphic_seven.grapic_data_struct[4].start_y=(u32)R_Board*sinf(yaw_angle360+PI)+Center_of_Circle_Y-OFFSET;
//			client_custom_graphic_seven.grapic_data_struct[4].end_x=(u32)(-R_Board*cosf(yaw_angle360+PI)+Center_of_Circle_X);
//			client_custom_graphic_seven.grapic_data_struct[4].end_y=(u32)(-R_Board*sinf(yaw_angle360+PI)+Center_of_Circle_Y-OFFSET);
//      client_custom_graphic_seven.grapic_data_struct[4].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[4].width=6;//竖	
//				
//			/*********************装甲板ID 4****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[5].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[5].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[5].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[1]=1;
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[2]=0;
//      client_custom_graphic_seven.grapic_data_struct[5].start_x=(u32)R_Board*cosf(yaw_angle360-PI/2)+Center_of_Circle_X+OFFSET;
//      client_custom_graphic_seven.grapic_data_struct[5].start_y=(u32)R_Board*sinf(yaw_angle360-PI/2)+Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[5].end_x=(u32)(-R_Board*cosf(yaw_angle360-PI/2)+Center_of_Circle_X+OFFSET);
//			client_custom_graphic_seven.grapic_data_struct[5].end_y=(u32)(-R_Board*sinf(yaw_angle360-PI/2)+Center_of_Circle_Y);
//      client_custom_graphic_seven.grapic_data_struct[5].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[5].width=6;//竖	
//				
//			/*********************云台朝向****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[6].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[6].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[6].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[1]=1;
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[2]=1;
//      client_custom_graphic_seven.grapic_data_struct[6].start_x=Center_of_Circle_X;
//      client_custom_graphic_seven.grapic_data_struct[6].start_y=Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[6].end_x=Center_of_Circle_X;
//			client_custom_graphic_seven.grapic_data_struct[6].end_y=Center_of_Circle_Y+300;
//      client_custom_graphic_seven.grapic_data_struct[6].color=UI_WHITE;
//      client_custom_graphic_seven.grapic_data_struct[6].width=5;//竖		
				
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_graphic_seven;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_graphic_seven),DN_REG_ID,tx_buf);
		}break;		
		case 7:
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id

			/*********************doors****************************************/	
			if(bullet_hatch.bullet_hatch_mode==OPEN)
			{client_custom_bomb_hatch_cover.grapic_data_struct[0].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[0].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[1]=1;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[2]=5;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_y=630;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[0].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[0].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[1]=1;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[2]=5;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_y=630;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].width=8;//竖
			}	
			
			
			if(chassis.ctrl_mode==CHASSIS_ROTATE)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[1].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[1].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[2]=9;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_y=720;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[1].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[1].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[2]=9;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_y=720;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].width=8;//竖
			}	
			
			if(gimbal_data.auto_aim_rotate_flag==0)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[2].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[2].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[2]=7;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[2].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[2].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[2]=7;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].width=8;//竖
			}	
			
			
			if(shoot.fric_wheel_run==1)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[3].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[3].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[2]=4;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_y=600;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[3].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[3].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[2]=4;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_y=600;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].width=8;//竖
			}	
			
			if(gimbal_data.auto_aim_rotate_flag==1)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[4].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[4].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_type=1;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[2]=2;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_x=800;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_y=400;	
      client_custom_bomb_hatch_cover.grapic_data_struct[4].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[4].operate_type=1; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[4].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_type=1;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[2]=2;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_x=800;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_y=400;	
      client_custom_bomb_hatch_cover.grapic_data_struct[4].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].width=8;//竖
			}
			
			*(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_bomb_hatch_cover;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_bomb_hatch_cover),DN_REG_ID,tx_buf);
		}break;
		
	/*******************************************************************************************************************************/
	/////////////////////////////////////////////////////////刷新循环////////////////////////////////////////////////////////////////
	/*******************************************************************************************************************************/
		case 8:  //电压数字值                   5
		{
			//-------------------------------------------电压数字值---------------------------------------------//
			ddata[0]=(uint8_t)0x0110;
      ddata[1]=0x0110>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id

      client_custom_character_voltage_num.grapic_data_struct.operate_type=2;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_character_voltage_num.grapic_data_struct.layer=1;   //图层
      client_custom_character_voltage_num.grapic_data_struct.graphic_type=7;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[0]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[1]=0;
      client_custom_character_voltage_num.grapic_data_struct.graphic_name[2]=5;

      client_custom_character_voltage_num.grapic_data_struct.start_x=695+200;
      client_custom_character_voltage_num.grapic_data_struct.start_y=75;
      client_custom_character_voltage_num.grapic_data_struct.width=WIDTH;
      client_custom_character_voltage_num.grapic_data_struct.start_angle=20;
      client_custom_character_voltage_num.grapic_data_struct.end_angle=4;
      sprintf(client_custom_character_voltage_num.data,"%f",usart_down_capacitance_message.cap_voltage);

      *(ext_client_custom_character_t*)(&ddata[6])=client_custom_character_voltage_num;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_character_voltage_num),DN_REG_ID,tx_buf);
		}break;
		case 9:  //电量长条                     6
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id

      client_custom_graphic_seven.grapic_data_struct[0].operate_type=2;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_graphic_seven.grapic_data_struct[0].layer=2;   //图层
      client_custom_graphic_seven.grapic_data_struct[0].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[0].graphic_name[2]=6;
																										 
      client_custom_graphic_seven.grapic_data_struct[0].start_x=695;
      client_custom_graphic_seven.grapic_data_struct[0].start_y=90;
      client_custom_graphic_seven.grapic_data_struct[0].end_y=90;
			
      if(usart_down_capacitance_message.cap_voltage>0.1f&&usart_down_capacitance_message.cap_voltage<=5.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=715;
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_RB;//UI_PINK;
        }
      else if(usart_down_capacitance_message.cap_voltage>5.0f)
        {
          client_custom_graphic_seven.grapic_data_struct[0].end_x=(u32)(735+((usart_down_capacitance_message.cap_voltage-5.0f))*725/24);
          client_custom_graphic_seven.grapic_data_struct[0].color=UI_YELLOW;//UI_PINK;
        }

      client_custom_graphic_seven.grapic_data_struct[0].width=30;
				
      /*********************准星显示****************************************/
      client_custom_graphic_seven.grapic_data_struct[1].operate_type=2;  //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
      client_custom_graphic_seven.grapic_data_struct[1].layer=1;   //图层
      client_custom_graphic_seven.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[1]=0;
      client_custom_graphic_seven.grapic_data_struct[1].graphic_name[2]=1;
      client_custom_graphic_seven.grapic_data_struct[1].start_x=960;
      client_custom_graphic_seven.grapic_data_struct[1].start_y=480;
			client_custom_graphic_seven.grapic_data_struct[1].radius=3;
      client_custom_graphic_seven.grapic_data_struct[1].width=5;//竖
			if(shoot.fric_wheel_run==1)
			{
			client_custom_graphic_seven.grapic_data_struct[1].color=UI_RB;
			}
			else 
			{			
			client_custom_graphic_seven.grapic_data_struct[1].color=UI_YELLOW;
			}
//					/*********************装甲板ID 1****************************************/	
//				
//			client_custom_graphic_seven.grapic_data_struct[2].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[2].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[2].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[2].graphic_name[2]=7;
//      client_custom_graphic_seven.grapic_data_struct[2].start_x=(u32)(Center_of_Circle_X+cosf(yaw_angle360)*R_Board);
//      client_custom_graphic_seven.grapic_data_struct[2].start_y=(u32)(R_Board*sinf(yaw_angle360)+Center_of_Circle_Y+OFFSET);
//			client_custom_graphic_seven.grapic_data_struct[2].end_x=(u32)(-R_Board*cosf(yaw_angle360)+Center_of_Circle_X);
//			client_custom_graphic_seven.grapic_data_struct[2].end_y=(u32)(-R_Board*sinf(yaw_angle360)+Center_of_Circle_Y+OFFSET);
//      client_custom_graphic_seven.grapic_data_struct[2].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[2].width=6;//竖	
//			
//			/*********************装甲板ID 2****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[3].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[3].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[3].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[3].graphic_name[2]=8;
//      client_custom_graphic_seven.grapic_data_struct[3].start_x=(u32)R_Board*cosf(yaw_angle360)+Center_of_Circle_X-OFFSET;
//      client_custom_graphic_seven.grapic_data_struct[3].start_y=(u32)R_Board*sinf(yaw_angle360)+Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[3].end_x=(u32)(-R_Board*cosf(yaw_angle360+PI/2)+Center_of_Circle_X-OFFSET);
//			client_custom_graphic_seven.grapic_data_struct[3].end_y=(u32)(-R_Board*sinf(yaw_angle360+PI/2)+Center_of_Circle_Y);
//      client_custom_graphic_seven.grapic_data_struct[3].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[3].width=6;//竖	
//			
//			/*********************装甲板ID 3****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[4].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[4].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[4].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[1]=0;
//      client_custom_graphic_seven.grapic_data_struct[4].graphic_name[2]=9;
//      client_custom_graphic_seven.grapic_data_struct[4].start_x=(u32)R_Board*cosf(yaw_angle360+PI/2)+Center_of_Circle_X;
//      client_custom_graphic_seven.grapic_data_struct[4].start_y=(u32)R_Board*sinf(yaw_angle360+PI/2)+Center_of_Circle_Y-OFFSET;
//			client_custom_graphic_seven.grapic_data_struct[4].end_x=(u32)(-R_Board*cosf(yaw_angle360+PI)+Center_of_Circle_X);
//			client_custom_graphic_seven.grapic_data_struct[4].end_y=(u32)(-R_Board*sinf(yaw_angle360+PI)+Center_of_Circle_Y-OFFSET);
//      client_custom_graphic_seven.grapic_data_struct[4].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[4].width=6;//竖	
//				
//			/*********************装甲板ID 4****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[5].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[5].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[5].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[1]=1;
//      client_custom_graphic_seven.grapic_data_struct[5].graphic_name[2]=0;
//      client_custom_graphic_seven.grapic_data_struct[5].start_x=(u32)R_Board*cosf(yaw_angle360+PI)+Center_of_Circle_X+OFFSET;
//      client_custom_graphic_seven.grapic_data_struct[5].start_y=(u32)R_Board*sinf(yaw_angle360+PI)+Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[5].end_x=(u32)(-R_Board*cosf(yaw_angle360-PI/2)+Center_of_Circle_X+OFFSET);
//			client_custom_graphic_seven.grapic_data_struct[5].end_y=(u32)(-R_Board*sinf(yaw_angle360-PI/2)+Center_of_Circle_Y);
//      client_custom_graphic_seven.grapic_data_struct[5].color=UI_RB;
//      client_custom_graphic_seven.grapic_data_struct[5].width=6;//竖	
//				
//			/*********************云台朝向****************************************/	
//			client_custom_graphic_seven.grapic_data_struct[6].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
//			client_custom_graphic_seven.grapic_data_struct[6].layer=1;   //图层
//			client_custom_graphic_seven.grapic_data_struct[6].graphic_type=0;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[0]=0;
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[1]=1;
//      client_custom_graphic_seven.grapic_data_struct[6].graphic_name[2]=1;
//      client_custom_graphic_seven.grapic_data_struct[6].start_x=Center_of_Circle_X;
//      client_custom_graphic_seven.grapic_data_struct[6].start_y=Center_of_Circle_Y;
//			client_custom_graphic_seven.grapic_data_struct[6].end_x=Center_of_Circle_X;
//			client_custom_graphic_seven.grapic_data_struct[6].end_y=Center_of_Circle_Y+300;
//      client_custom_graphic_seven.grapic_data_struct[6].color=UI_WHITE;
//      client_custom_graphic_seven.grapic_data_struct[6].width=5;//竖			
			
      *(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_graphic_seven;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_graphic_seven),DN_REG_ID,tx_buf);
		}break;
 	
		case 10:
		{
      ddata[0]=0x0104;
      ddata[1]=0x0104>>8;	 //数据内容id
      //0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      ddata[2]=judge_rece_mesg.game_robot_state.robot_id;
      ddata[3]=judge_rece_mesg.game_robot_state.robot_id>>8;    //机器人id
      ddata[4]=client_custom_ID;
      ddata[5]=client_custom_ID>>8;       //客户端id

			/*********************doors****************************************/	
			if(bullet_hatch.bullet_hatch_mode==OPEN)
			{client_custom_bomb_hatch_cover.grapic_data_struct[0].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[0].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[1]=1;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[2]=5;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_y=630;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[0].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[0].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[1]=1;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].graphic_name[2]=5;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].start_y=630;
			client_custom_bomb_hatch_cover.grapic_data_struct[0].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[0].width=8;//竖
			}	
			
			
			if(chassis.ctrl_mode==CHASSIS_ROTATE)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[1].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[1].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[2]=9;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_y=720;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[1].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[1].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].graphic_name[2]=9;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].start_y=720;
			client_custom_bomb_hatch_cover.grapic_data_struct[1].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[1].width=8;//竖
			}	
			
			if(gimbal_data.auto_aim_rotate_flag==1)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[2].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[2].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[2]=7;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[2].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[2].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].graphic_name[2]=7;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[2].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[2].width=8;//竖
			}	
			
						
			if(shoot.fric_wheel_run==1)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[3].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[3].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[2]=4;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_y=590;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[3].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[3].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_type=2;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].graphic_name[2]=4;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].start_y=590;
			client_custom_bomb_hatch_cover.grapic_data_struct[3].radius =7;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[3].width=8;//竖
			}	

			if(gimbal_data.auto_aim_rotate_flag==1)
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[4].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[4].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_type=1;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[2]=2;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_x=800;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_y=400;	
      client_custom_bomb_hatch_cover.grapic_data_struct[4].color=UI_WHITE;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].width=8;//竖
			}
			else
			{
			client_custom_bomb_hatch_cover.grapic_data_struct[4].operate_type=2; //1 增加 2修改图形 3删除单个图形 5删除一个图层的图形 6删除所有图形
			client_custom_bomb_hatch_cover.grapic_data_struct[4].layer=1;   //图层
			client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_type=1;  //0 直线 1矩形 2整圆 3椭圆 4圆弧 5浮点数 6整数型 7字符
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[0]=0;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[1]=2;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].graphic_name[2]=2;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_x=360;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].start_y=675;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_x=800;
			client_custom_bomb_hatch_cover.grapic_data_struct[4].end_y=400;	
      client_custom_bomb_hatch_cover.grapic_data_struct[4].color=UI_YELLOW;
      client_custom_bomb_hatch_cover.grapic_data_struct[4].width=8;//竖
			}
			*(ext_client_custom_graphic_seven_t*)(&ddata[6])=client_custom_bomb_hatch_cover;
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, ddata,6+sizeof(client_custom_bomb_hatch_cover),DN_REG_ID,tx_buf);
		}break;
    default:
    break;
    }
	draw_cnt++;
	draw_int++;
  if(draw_cnt>10)//在需要刷新的图层刷新
    draw_cnt=8;
	
}

void delete_Coverage(u8 coverage)
{
  ddata[6]=4;//1增加2修改3删除单个4删除图层5删除所有
  ddata[13]=coverage;//图层0-9
}
