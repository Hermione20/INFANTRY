#include "uart_cha_transmit.h"
/*通过下层板串口2发送超电电容电量至上层板，为UI提供信息*/


uart_cha_data_t uart_cha_data;
uart_cap_down_data_t uart_cap_down_data;

	 uint8_t USART2_DMA_TX_BUF[sizeof(uart_cha_data_t)];
/**
 * @brief  发送超电数据
 * @param  S： 发送超电数据结构体
 * @call   control_task()
 * */
void Uart_down_to_up_send(uart_cap_down_data_t *S)
{
	
	S->cap_voltage=usart_capacitance_message.cap_voltage_filte;	
	USART2_dma_start1(&uart_cap_down_data,USART2_DMA_TX_BUF,USART2_TX_BUF_LENGTH);
	
}

