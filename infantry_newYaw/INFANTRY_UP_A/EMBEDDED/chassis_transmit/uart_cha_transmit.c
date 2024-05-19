#include "uart_cha_transmit.h"

uart_cha_data_t uart_cha_data;
usart_down_capacitance_message_t usart_down_capacitance_message;


extern void USART2_dma_start(uart_cha_data_t *uart_cha_data);

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
								u8 climbing_mode)
{
	uart_cha_data.RC_inputmode = RC_inputmode;
	uart_cha_data.if_follow_gim = if_follow_gim;
	uart_cha_data.speed_mode = speed_mode;
	uart_cha_data.chassis_mode = chassis_mode;
	uart_cha_data.yaw_Encoder_ecd_angle = yaw_encoder_angle;
	uart_cha_data.yaw_Encoder_filter_rate = yaw_encoder_filter_rate;
	uart_cha_data.x = x;
	uart_cha_data.y = y;
	uart_cha_data.rotate_speed = rotate_speed;
	uart_cha_data.chassis_power = chassis_power;
	uart_cha_data.chassis_power_buffer = chassis_power_buffer;
	uart_cha_data.chassis_power_limit = chassis_power_limit;
  uart_cha_data.mains_power_chassis_output=mains_power_chassis_output;
	uart_cha_data.robot_level=judge_rece_mesg.game_robot_state.robot_level;
	uart_cha_data.climbing_mode=chassis.climbing_mode;
  USART2_dma_start(&uart_cha_data);
}


/**
 * @brief  超电接收数据处理
 * @param  v： 接收超电数据结构体
 * @param  data： 接收缓冲数组
 * @call   USART2_IRQHandler()
 * */
void SuperCap_message_Process(volatile usart_down_capacitance_message_t *v,uint8_t *data)
{
	memcpy((uint8_t *)v,data,sizeof(usart_down_capacitance_message_t));
}
