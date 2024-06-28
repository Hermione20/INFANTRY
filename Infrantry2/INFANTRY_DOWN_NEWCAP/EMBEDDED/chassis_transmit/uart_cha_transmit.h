#ifndef __UART_CHA_TRANSMIT_H
#define __UART_CHA_TRANSMIT_H
#include "public.h"

typedef __packed struct
{
	u8 RC_inputmode;
	u8 if_follow_gim;
	u8 speed_mode;
	u8 chassis_mode;
	u8 chassis_power_limit;	
	u8 mains_power_chassis_output;
	u8 robot_level;
	u8 climbing_mode;
	int16_t yaw_Encoder_filter_rate;
	int16_t x;
	int16_t y;
	int16_t rotate_speed;
	int16_t chassis_power;			
	uint16_t chassis_power_buffer;	
	double yaw_Encoder_ecd_angle;
} uart_cha_data_t;


typedef  __packed struct
{
   float cap_voltage;
}uart_cap_down_data_t;


extern uint8_t USART2_DMA_TX_BUF[sizeof(uart_cha_data_t)];
void USART2_dma_start1(uart_cap_down_data_t *uart_cap_down_data,uint8_t* TX_BUF,uint16_t Counter);
//void USART2_dma_start(uart_cha_data_t *uart_cha_data,uint8_t* TX_BUF,uint16_t Counter);
extern uart_cha_data_t uart_cha_data;
extern uart_cap_down_data_t uart_cap_down_data;
void Uart_down_to_up_send(uart_cap_down_data_t *S);





#endif

