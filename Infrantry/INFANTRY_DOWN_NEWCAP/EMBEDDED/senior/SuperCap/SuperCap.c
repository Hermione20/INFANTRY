#include "SuperCap.h"

SuperCap_Send_t Super_Cap_Send;//�������ݽṹ��
uint8_t Capacitance_Message_Buf[100];	//���ͻ�������
volatile usart_capacitance_message_t usart_capacitance_message;//�������ݽṹ��


volatile can_capacitance_message_t can_capacitance_message;


/************************************* USART ͨ��  ************************************************/
/**
 * @brief  ���緢������
 * @param  S�� ���ͳ������ݽṹ��
 * @call   control_task()
 * */
void POWER_Control(SuperCap_Send_t *S)
{
	if(uart_cha_data.RC_inputmode==STOP||uart_cha_data.RC_inputmode==0)
		S->Stop_Control_Flag=0;
	else 
		S->Stop_Control_Flag=1;
	
	S->chassis_power_buffer=uart_cha_data.chassis_power_buffer;
	S->chassis_power_limit =uart_cha_data.chassis_power_limit;

	memcpy(&Capacitance_Message_Buf,(uint8_t *)S,sizeof(SuperCap_Send_t));
	Uart3SendBytesInfoProc((uint8_t *)Capacitance_Message_Buf,sizeof(Capacitance_Message_Buf));
}

/**
 * @brief  ����������ݴ���
 * @param  v�� ���ճ������ݽṹ��
 * @param  data�� ���ջ�������
 * @call   USART3_IRQHandler()
 * */
void SuperCap_message_Process(volatile usart_capacitance_message_t *v,uint8_t *data)
{
	memcpy((uint8_t *)v,data,sizeof(usart_capacitance_message));
}


/************************************* CAN ͨ��  ************************************************/

void Can_SuperCap_message_Process(volatile can_capacitance_message_t *v,CanRxMsg * msg)
{
	switch (msg->StdId)
	{
		case 0x123:
		{
			memcpy((uint8_t *)v,msg->Data,8);
		}
		break;

		default:
			break;
	}
}


    
void CAN_POWER_Control(CAN_TypeDef *CANx ,SuperCap_Send_t *SC)
{
		if(uart_cha_data.RC_inputmode==STOP||uart_cha_data.RC_inputmode==0)
			SC->Stop_Control_Flag=0;
		else 
			SC->Stop_Control_Flag=1;
		
		SC->chassis_power_buffer=uart_cha_data.chassis_power_buffer;
		SC->chassis_power_limit =uart_cha_data.chassis_power_limit;

		memcpy(&Capacitance_Message_Buf,(uint8_t *)SC,sizeof(SuperCap_Send_t));
		
    CanTxMsg tx_message;
    tx_message.StdId = 0x300;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
		memcpy(tx_message.Data,(uint8_t *)SC,sizeof(SuperCap_Send_t));
    CAN_Transmit(CANx,&tx_message);
}
