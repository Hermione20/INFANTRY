#include "client.h"
u16 draw_data_ID=0x0101;
u16 data_ID=0xD180;
u16 client_custom_ID=0;
uint8_t dddata[120];
uint8_t  tx_buf[150];
#define WIDTH    3
UI_t UI=UI_DEFAULT;

/*创建图形对象*/
interaction_figure_t _0=ARC(ADD,0,0,1,960,540,140*3-30,140*3-30,0,359,3,1,UI_YELLOW);
interaction_figure_t _1=CIRCLE(ADD,0,0,2,960,505,3,5,1,UI_YELLOW);
interaction_figure_t _2=CHARACTER(ADD,0,0,3,200,740,20,20,WIDTH,0,UI_GREEN);/*ROTATE*/
interaction_figure_t _3=CHARACTER(ADD,0,0,4,200,690,20,20,WIDTH,1,UI_GREEN);/*DOOR*/
interaction_figure_t _4=CHARACTER(ADD,0,0,5,200,640,20,20,WIDTH,1,UI_GREEN);/*FRICTION*/
interaction_figure_t _5=CHARACTER(ADD,0,0,6,200,690,20,20,WIDTH,1,UI_GREEN);/*DEFEND*/
interaction_figure_t _6=CHARACTER(ADD,0,0,7,200,590,20,200,WIDTH,0,UI_GREEN);/*VOTAGE*/

interaction_figure_t _00=CIRCLE(ADD,0,0,8,380,680,7,8,1,UI_ORANGE);/*DOOR*/
interaction_figure_t _01=CIRCLE(ADD,0,0,9,380,730,7,8,1,UI_ORANGE);/*ROTATE*/
interaction_figure_t _02=CIRCLE(ADD,0,1,0,380,675,7,8,1,UI_ORANGE);/*DEFEND*/
interaction_figure_t _03=CIRCLE(ADD,0,1,1,380,630,7,8,1,UI_ORANGE);/*FRICTION*/
interaction_figure_t _04=CIRCLE(ADD,0,1,2,960,505,3,5,1,UI_RB);
interaction_figure_t _05=CHARACTER(ADD,0,1,3,200,600 ,10,30,WIDTH,1,UI_WHITE);/*电压数字*/
interaction_figure_t _06=CHARACTER(ADD,0,1,4,200,540 ,20,40,WIDTH,1,UI_CYAN);/*累计发弹量*/

interaction_figure_t _001=FLOAT_NUM(ADD,0,1,5,350,590,0,20,WIDTH,1,UI_ORANGE);/*usart_down_capacitance_message.cap_voltage*/
interaction_figure_t _002=FLOAT_NUM(ADD,0,1,6,200,540,0,20,WIDTH,1,UI_CYAN);/*already_shoot*/
interaction_figure_t _003=ARC(ADD,0,1,7,960,540,70,70,0,359,3,0,UI_RB);/*heat*/
interaction_figure_t _004=ARC(ADD,0,1,8,960,540,140*3-40,140*3-40,0,359,14,2,UI_CYAN);/*pitch*/
interaction_figure_t _005=ARC(ADD,0,1,9,1500,750,40,40,0,359,15,0,UI_CYAN);/*yaw*/
interaction_figure_t _006=ARC(ADD,0,2,0,880,75,25,25,0,359,5,0,UI_CYAN);/*big buff*/
interaction_figure_t _007=ARC(ADD,0,2,1,960,75,25,25,0,359,5,0,UI_CYAN);/*small buff*/
interaction_figure_t _008=ARC(ADD,0,2,2,1040,75,25,25,0,359,5,0,UI_CYAN);/*auto shoot*/

interaction_figure_t _10=ARC(ADD,0,2,3,960,540,140*3-30,140*3-30,0,359,3,0,UI_BLACK);/*电容警示圈*/
interaction_figure_t _11=ARC(ADD,0,2,4,1500,750,40,40,0,359,15,0,UI_RB);/*yaw底圈*/
interaction_figure_t _12=ARC(ADD,0,2,5,880,75,20,20,0,359,10,0,UI_ORANGE);/*big buff*/
interaction_figure_t _13=ARC(ADD,0,2,6,960,75,20,20,0,359,5,0,UI_YELLOW);/*small buff*/
interaction_figure_t _14=ARC(ADD,0,2,7,1040,75,20,20,0,359,2,0,UI_CYAN);/*auto shoot*/
interaction_figure_t _15=FLOAT_NUM(ADD,0,2,8,1350+20,540,0,15,WIDTH,1,UI_WHITE);/*电压数字*/
interaction_figure_t _16=FLOAT_NUM(ADD,0,2,8,1350,735,0,15,WIDTH,1,UI_WHITE);/*累计发弹量*/

/*创建 组合图形对象*/
interaction_figure_4_t A;
interaction_figure_4_t AA;
interaction_figure_4_t AB;

/*创建 字符对象*/
client_custom_character_t B;uint8_t dataB[]="ROTATE:";
client_custom_character_t C;uint8_t dataC[9]={'F','R','I','C','T','I','O','N',':'};
client_custom_character_t D;uint8_t dataD[]="DOOR:";
client_custom_character_t E;uint8_t dataE[]="DEFEND:";
client_custom_character_t F;uint8_t dataF[]="VOTAGE:";
client_custom_character_t G;
client_custom_character_t H;

/*UI刷新主函数*/
void Client_Send_Handle()
{

	if(UI.circle_360<360)
	{UI.circle_360+=40;}
	else
	{UI.circle_360-=360;}
	
  UI.id=judge_rece_mesg.game_robot_state.robot_id;
  switch(UI.id)
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
	
	switch(UI.cnt)
		{
		case 1:/*静态显示*/
		{
			UI.ADD_7Graph(A,_0,_1,_00,_01,_001,_03,_04);
		}break;
		case 2:
		{
			UI.ADD_Char(F,_6,dataF,7);
		}break;
		case 3:
		{
			UI.ADD_Char(C,_4,dataC,9);
		}break;
		case 4:
		{
			UI.ADD_Char(B,_2,dataB,7);
		}break;
		case 5:
		{
			UI.ADD_Char(D,_3,dataD,5);
		}break;
		case 6:
		{
			UI.ADD_7Graph(AB,_11,_12,_13,_14,_15,_16,_10);
		}break;
		case 7:
		{
			UI.ADD_7Graph(AA,_002,_003,_004,_005,_006,_007,_008);
		}break;
		case 8:/*动态显示*/
		{
			UI.MODIFY_7Graph_0(A,_0,_1,_00,_01,_001,_03,_04);
		}break;
		case 9:
		{
			UI.MODIFY_7Graph_1(AA,_002,_003,_004,_005,_006,_007,_008);
		}break;
		
		default:
     break;
    }
		
	UI.cnt++;
  if(UI.cnt>9)/*在需要刷新的图层刷新*/
     UI.cnt=8;
}



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



void ADD_Character(client_custom_character_t _0,interaction_figure_t __0,uint8_t *data0,uint8_t size0)
{
		robot_interaction_data_t UI_data;

		UI_data.id_data.data_cmd_id=0x0110;
		UI_data.id_data.sender_id =judge_rece_mesg.game_robot_state.robot_id;
    UI_data.id_data.receiver_id=client_custom_ID; //客户端id

		memcpy((uint8_t *)dddata,(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));
	
		_0.interaction_figure=__0;
		memcpy(_0.data,data0,size0);
		*(client_custom_character_t*)(&dddata[6])=_0;
//		memcpy((uint8_t *)&UI_data.user_data,(uint8_t *)&_0,sizeof(client_custom_character_t));
	
		memcpy((uint8_t *)(dddata+6+sizeof(client_custom_character_t)),(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));
	
		data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID,dddata,2*sizeof(UI_data.id_data)+sizeof(client_custom_character_t),DN_REG_ID,tx_buf);
}

void ADD_7_Graph(interaction_figure_4_t _7,interaction_figure_t _0,interaction_figure_t _1,interaction_figure_t _2,interaction_figure_t _3,interaction_figure_t _4,interaction_figure_t _5,interaction_figure_t _6)
{
     robot_interaction_data_t UI_data;
	
			UI_data.id_data.data_cmd_id=0x0104;//0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
      UI_data.id_data.sender_id =judge_rece_mesg.game_robot_state.robot_id;
      UI_data.id_data.receiver_id=client_custom_ID; //客户端id

			_7.interaction_figure[0]=_0;
			_7.interaction_figure[1]=_1;
			_7.interaction_figure[2]=_2;
			_7.interaction_figure[3]=_3;
			_7.interaction_figure[4]=_4;
			_7.interaction_figure[5]=_5;
			_7.interaction_figure[6]=_6;

			memcpy(dddata,(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));
		  *(interaction_figure_4_t*)(&dddata[6])=_7;
//			memcpy(dddata+sizeof(UI_data.id_data),(interaction_figure_4_t *)&_7,sizeof(interaction_figure_4_t));
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID,dddata,sizeof(UI_data.id_data)+sizeof(interaction_figure_4_t),DN_REG_ID,tx_buf);
		}

void MODIFY_2_Character_Num(client_custom_character_t _0,interaction_figure_t __0,float data0,client_custom_character_t _1,interaction_figure_t __1,float data1)
{
		robot_interaction_data_t UI_data;

		UI_data.id_data.data_cmd_id=0x0110;
		UI_data.id_data.sender_id =judge_rece_mesg.game_robot_state.robot_id;
    UI_data.id_data.receiver_id=client_custom_ID; //客户端id
	
    memcpy(dddata,(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));	
	
		_0.interaction_figure=__0;
		_0.interaction_figure=__1;
	
		_0.interaction_figure.operate_tpye=2;
		sprintf((char *)_0.data,"%f",data0);
		*(client_custom_character_t*)(&dddata[6])=_0;
	
		_1.interaction_figure.operate_tpye=2;
		sprintf((char *)_1.data,"%f",data1);
		*(client_custom_character_t*)(&dddata[6+sizeof(client_custom_character_t)])=_1;
//		memcpy((uint8_t *)&UI_data.user_data,(uint8_t *)&_0,sizeof(client_custom_character_t));
//		memcpy((uint8_t *)&UI_data.user_data+sizeof(client_custom_character_t),(uint8_t *)&_1,sizeof(client_custom_character_t));
	 data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID,dddata ,sizeof(UI_data.id_data)+2*sizeof(client_custom_character_t),DN_REG_ID,tx_buf);
}

void MODIFY_7_Graph_DIY(interaction_figure_4_t _7,interaction_figure_t _0,interaction_figure_t _1,interaction_figure_t _2,interaction_figure_t _3,interaction_figure_t _4,interaction_figure_t _5,interaction_figure_t _6)
{
		robot_interaction_data_t UI_data;
	
		UI_data.id_data.data_cmd_id=0x0104;//0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
    UI_data.id_data.sender_id =judge_rece_mesg.game_robot_state.robot_id;
    UI_data.id_data.receiver_id=client_custom_ID; //客户端id
	
		_7.interaction_figure[0]=_0;
		_7.interaction_figure[1]=_1;
		_7.interaction_figure[2]=_2;
		_7.interaction_figure[3]=_3;
		_7.interaction_figure[4]=_4;
		_7.interaction_figure[5]=_5;
		_7.interaction_figure[6]=_6;
	
		_7.interaction_figure[0].operate_tpye=MODIFY;
		_7.interaction_figure[1].operate_tpye=MODIFY;
		_7.interaction_figure[2].operate_tpye=MODIFY;
		_7.interaction_figure[3].operate_tpye=MODIFY;
		_7.interaction_figure[4].operate_tpye=MODIFY;
		_7.interaction_figure[5].operate_tpye=MODIFY;
		_7.interaction_figure[6].operate_tpye=MODIFY;
/*第1个图形*/
		_7.interaction_figure[0].details_b=usart_down_capacitance_message.cap_voltage*359.8f/28.0f;
		if(usart_down_capacitance_message.cap_voltage<=0)
			{
					_7.interaction_figure[0].color=UI_YELLOW;
					_7.interaction_figure[0].details_b=360;
			}
		
		if(usart_down_capacitance_message.cap_voltage>5)
			{
				_7.interaction_figure[0].color=UI_YELLOW;
			}
		else if(usart_down_capacitance_message.cap_voltage<5)
		  {
				_7.interaction_figure[0].color=UI_RB;

		  }
		 
/*第2个图形*/	
		if(shoot.fric_wheel_run==1)
			{
				_7.interaction_figure[1].color=UI_RB;
			}
		else
			{
				_7.interaction_figure[1].color=UI_YELLOW;
			}
/*第3个图形*/			
		if(bullet_hatch.bullet_hatch_mode==OPEN)
			{
				_7.interaction_figure[2].color=UI_ORANGE;
			}
		else
			{
				_7.interaction_figure[2].color=UI_PURPLE;
			}
/*第4个图形*/			
		if(chassis.ctrl_mode==CHASSIS_ROTATE)
			{
				_7.interaction_figure[3].color=UI_ORANGE;
			}
		else
			{
				_7.interaction_figure[3].color=UI_PURPLE;
			}
/*第5个图形 电容值*/			
			uint32_t  cap_temp=(usart_down_capacitance_message.cap_voltage*1000.0f)*100/28;
			_7.interaction_figure[4].details_c=cap_temp;
		  _7.interaction_figure[4].details_d=cap_temp>>10;
			_7.interaction_figure[4].details_e=cap_temp>>21;
/*第6个图形*/			
		if(shoot.fric_wheel_run==1)
			{
				_7.interaction_figure[5].color=UI_ORANGE;
			}
		else
			{
				_7.interaction_figure[5].color=UI_PURPLE;
			}
/*第7个图形*/			
		if(shoot.fric_wheel_run==1)/*重复的*/
			{
				_7.interaction_figure[6].color=UI_RB;
			}
		else
			{
				_7.interaction_figure[6].color=UI_YELLOW;
			}
			
			memcpy((uint8_t *)dddata,(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));
			*(interaction_figure_4_t*)(&dddata[6])=_7;
//			memcpy((uint8_t *)(dddata+sizeof(UI_data.id_data)),(interaction_figure_4_t*)&_7,sizeof(interaction_figure_4_t));
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, dddata,sizeof(UI_data.id_data)+sizeof(interaction_figure_4_t),DN_REG_ID,tx_buf);
}

void MODIFY_7_Graph_DIY1(interaction_figure_4_t _7,interaction_figure_t _0,interaction_figure_t _1,interaction_figure_t _2,interaction_figure_t _3,interaction_figure_t _4,interaction_figure_t _5,interaction_figure_t _6)
{
		robot_interaction_data_t UI_data;
	
		UI_data.id_data.data_cmd_id=0x0104;//0x0100  删除图形 0x0101 绘制一个图形 0x0102 绘制二个图形 0x0103 绘制五个图形 0x0104绘制七个图形 0x0110客户端绘制字符图形
    UI_data.id_data.sender_id =judge_rece_mesg.game_robot_state.robot_id;
    UI_data.id_data.receiver_id=client_custom_ID; //客户端id
	
		_7.interaction_figure[0]=_0;
		_7.interaction_figure[1]=_1;
		_7.interaction_figure[2]=_2;
		_7.interaction_figure[3]=_3;
		_7.interaction_figure[4]=_4;
		_7.interaction_figure[5]=_5;
		_7.interaction_figure[6]=_6;
	
		_7.interaction_figure[0].operate_tpye=MODIFY;
		_7.interaction_figure[1].operate_tpye=MODIFY;
		_7.interaction_figure[2].operate_tpye=MODIFY;
		_7.interaction_figure[3].operate_tpye=MODIFY;
		_7.interaction_figure[4].operate_tpye=MODIFY;
		_7.interaction_figure[5].operate_tpye=MODIFY;
		_7.interaction_figure[6].operate_tpye=MODIFY;
		
/*第1个图形 累计发弹数*/
			uint32_t  shoot_temp=(already_shoot*1000.0f);
			_7.interaction_figure[0].details_c=shoot_temp;
		  _7.interaction_figure[0].details_d=shoot_temp>>10;
			_7.interaction_figure[0].details_e=shoot_temp>>21;
		 
/*第2个图形 预计发弹数*/	
		_7.interaction_figure[1].details_b=shoot.will_time_shoot*359.8f/50.0f;
		if(shoot.will_time_shoot>2)
			{
				_7.interaction_figure[1].color=UI_CYAN;
			}
		else if(shoot.will_time_shoot<=2)
		  {
				_7.interaction_figure[1].color=UI_RB;
		  }
			
/*第3个图形 pitch*/	
		_7.interaction_figure[2].details_a=90-3+gimbal_gyro .pitch_Angle;			
		_7.interaction_figure[2].details_b=90+3+gimbal_gyro .pitch_Angle;
			
/*第4个图形 yaw*/	
			float yaw__180_180;
			float yaw_0_360	=fmod(yaw_Encoder.ecd_angle*YAW_POLARITY,360);	
			if(yaw_0_360<0){yaw_0_360+=360;}
				if(yaw_0_360>=180)/*将0-2PI转换到-PI-PI范围内*/
					{yaw__180_180=yaw_0_360-360;}
				else
					{yaw__180_180=yaw_0_360;}
		
		_7.interaction_figure[3].details_a=yaw_0_360+15;//gimbal_gyro.yaw_Angle+15;		

		if(yaw_0_360+345>360)
			yaw_0_360=yaw_0_360-360;
		_7.interaction_figure[3].details_b=yaw_0_360+345;//gimbal_gyro.yaw_Angle+345;
		
/*第5个图形 big buff*/			
		if(gimbal_data.ctrl_mode==GIMBAL_AUTO_BIG_BUFF)
		 {
			  _7.interaction_figure[4].width=10;
 
				_7.interaction_figure[4].details_a=UI.circle_360;
				_7.interaction_figure[4].details_b=UI.circle_360-50;						
		 }
		else
		 {
			  _7.interaction_figure[4].width=5;
				_7.interaction_figure[4].details_a=0;
				_7.interaction_figure[4].details_b=360;
		 }
		 
/*第6个图形 small buf*/			
		if(gimbal_data.ctrl_mode==GIMBAL_AUTO_SMALL_BUFF)
		 {
				_7.interaction_figure[5].width=10;
				_7.interaction_figure[5].details_a=UI.circle_360;
				_7.interaction_figure[5].details_b=UI.circle_360-50;						
		 }
		else
		 {
			  _7.interaction_figure[5].width=5;
				_7.interaction_figure[5].details_a=0;
				_7.interaction_figure[5].details_b=360;
		 }
/*第7个图形 auto shoot*/			
				if(gimbal_data.auto_aim_rotate_flag==1)
		 {
			 _7.interaction_figure[6].width=10;
			 _7.interaction_figure[6].details_a=UI.circle_360;
			 _7.interaction_figure[6].details_b=UI.circle_360-50;						

		 }
		else
		 {
			  _7.interaction_figure[6].width=5;
				_7.interaction_figure[6].details_a=0;
				_7.interaction_figure[6].details_b=360;
		 }
			
			memcpy((uint8_t *)dddata,(uint8_t *)&UI_data.id_data,sizeof(UI_data.id_data));
			*(interaction_figure_4_t*)(&dddata[6])=_7;
//			memcpy((uint8_t *)(dddata+sizeof(UI_data.id_data)),(interaction_figure_4_t*)&_7,sizeof(interaction_figure_4_t));
      data_upload_handle(STUDENT_INTERACTIVE_HEADER_DATA_ID, dddata,sizeof(UI_data.id_data)+sizeof(interaction_figure_4_t),DN_REG_ID,tx_buf);
}


uint8_t* protocol_packet_pack(uint16_t cmd_id, uint8_t *p_data, uint16_t len, uint8_t sof, uint8_t *tx_buf)  
{
  uint16_t frame_length = HEADER_LEN + CMD_LEN + len + CRC_LEN;
  frame_header_t *p_header = (frame_header_t*)tx_buf;           //??frame_header?????????tx_buf??
  p_header->sof          = sof;
  p_header->data_length  = len;
  p_header->seq          = 0;

  Append_CRC8_Check_Sum(tx_buf, HEADER_LEN);
  memcpy(&tx_buf[HEADER_LEN], (uint8_t*)&cmd_id, CMD_LEN);      //??ID????tx_buf??
  memcpy(&tx_buf[HEADER_LEN + CMD_LEN], p_data, len);           //?????????tx_buf?У??????λ??16λcrcУ??
  Append_CRC16_Check_Sum(tx_buf, frame_length);

  return tx_buf;                                                //?????tx_buf????????????
}

void data_upload_handle(uint16_t cmd_id, uint8_t *p_data, uint16_t len, uint8_t sof, uint8_t *tx_buf)
{
  uint16_t frame_length = HEADER_LEN + CMD_LEN + len + CRC_LEN;
  
  protocol_packet_pack(cmd_id, p_data, len, sof, tx_buf);   //crcУ??
  if (sof == UP_REG_ID)
  // write_uart_blocking(&COMPUTER_HUART, tx_buf, frame_length);
	 {}
  else if (sof == DN_REG_ID)
	{
		usart5.Send_bytes(&usart5,tx_buf,frame_length);			
	}
}
	