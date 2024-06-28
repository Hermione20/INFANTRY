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


typedef __packed struct 
{
	
	float cap_voltage;

}usart_down_capacitance_message_t;




extern uart_cha_data_t uart_cha_data;
extern usart_down_capacitance_message_t usart_down_capacitance_message;

void uart_cha_task(u8 RC_inputmode,
							  u8 if_follow_gim,
                u8 speed_mode,
                u8 chassis_mode,
                double yaw_encoder_angle,
                int16_t yaw_encoder_filter_rate,
                int16_t x,
                int16_t y,
                int16_t rotate_speed,
                int16_t chassis_power,
                uint16_t chassis_power_buffer,
                u8 chassis_power_limit,
								u8 mains_power_chassis_output,
								u8 robot_level,
								u8 climbing_mode);
								
								
void SuperCap_message_Process(volatile usart_down_capacitance_message_t *v,uint8_t *data);

#endif

