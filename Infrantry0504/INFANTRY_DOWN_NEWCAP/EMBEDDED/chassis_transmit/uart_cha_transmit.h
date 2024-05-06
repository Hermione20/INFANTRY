#ifndef __UART_CHA_TRANSMIT_H
#define __UART_CHA_TRANSMIT_H
#include "public.h"

typedef __packed struct
{
	u8 RC_inputmode;
	u8 if_follow_gim;
	u8 speed_mode;
	u8 chassis_mode;
	u8 chassis_power_limit;			//
	int16_t yaw_Encoder_filter_rate;
	int16_t x;
	int16_t y;
	int16_t rotate_speed;
	int16_t chassis_power;			//
	uint16_t chassis_power_buffer;	//
	double yaw_Encoder_ecd_angle;
} uart_cha_data_t;

extern uart_cha_data_t uart_cha_data;

#endif

