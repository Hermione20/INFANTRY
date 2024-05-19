/* Includes ------------------------------------------------------------------*/
#include "can.h"
/**
      -@- �����ֻʹ��CAN2�����������CAN1ʱ��
       
      (#) CAN pins configuration
        (++) ����CAN gpioʱ�ӣ�ʹ�����¹���:
							RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOx,ENABLE); 
        (++) ʹ�����¹��ܽ��漰��CAN�������ӵ�AF9
							GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_CANx);
        (++) ͨ�����ý���ЩCAN��������Ϊ���ù���ģʽ
							����GPIO_Init();
							ʹ��CAN_Init()�ͳ�ʼ��������CAN
							CAN_FilterInit()������
			(#)ʹ��CAN_Transmit()�������������CAN֡��

			(#)ʹ��CAN_TransmitStatus()���CAN֡�Ĵ��亯����

			(#)ʹ��CAN_CancelTransmit()ȡ��CAN֡�Ĵ��亯����
				 ʹ��can_receive()��������һ��CAN֡��

		  (#)ʹ��CAN_FIFORelease()�����ͷŽ���fifo��

			(#)���صȴ�����֡�ĸ���
					CAN_MessagePending()������
											 
      (#) Ҫ����CAN�¼�������ʹ���������ַ���֮һ:
        (++) ʹ��CAN_GetFlagStatus()�������CAN��־�� 
        (++) ͨ����ʼ���׶ε�CAN_ITConfig()�������ж������е�CAN_GetITStatus()����ʹ��CAN�ж�������¼��Ƿ�����
             ���һ����־����Ӧ��ʹ��CAN_ClearFlag()������������ڼ���ж��¼�����Ӧ��ʹ��CAN_ClearITPendingBit()�����������  
 
/* Variables_definination-----------------------------------------------------------------------------------------------*/
u8 CAN1_receive_buf[20];
u8 CAN2_receive_buf[20];
/*----------------------------------------------------------------------------------------------------------------------*/

#if EN_CAN1
/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/
void CAN1_Mode_Init(u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{   
		GPIO_InitTypeDef       gpio;
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
//    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 0;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);    
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = DISABLE;
    can.CAN_Mode = mode;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = tbs1;
    can.CAN_BS2 = tbs2;
    can.CAN_Prescaler = brp;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber=0;
		can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
		can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
		can_filter.CAN_FilterIdHigh=0x0000;
		can_filter.CAN_FilterIdLow=0x0000;
		can_filter.CAN_FilterMaskIdHigh=0x0000;
		can_filter.CAN_FilterMaskIdLow=0x0000;
		can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
		can_filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&can_filter);
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
//    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

void CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
		{
				CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		}
}

/*************************************************************************
                          CAN1_RX0_IRQHandler
������CAN1�Ľ����жϺ���
*************************************************************************/

void CAN1_RX0_IRQHandler(void)
{    
//    TIM_Cmd(TIM4, ENABLE);
//    start = TIM4->CNT;
         
			CanRxMsg rx_message;
		if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
		{
				CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
				CAN_ClearFlag(CAN1, CAN_FLAG_FF0); 
				CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
				CAN1_Data_Receive_Process
		}
//        run_time = TIM4->CNT - start;
//        TIM_Cmd(TIM4, DISABLE);
}
#endif

#if EN_CAN2
/*----CAN2_TX-----PB13----*/
/*----CAN2_RX-----PB12----*/

void CAN2_Mode_Init(u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 ;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOB, &gpio);

    nvic.NVIC_IRQChannel = CAN2_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN2_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = DISABLE;    
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = DISABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = tbs1;
    can.CAN_BS2 = tbs2;
    can.CAN_Prescaler = brp;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);
    
    can_filter.CAN_FilterNumber=14;
    can_filter.CAN_FilterMode=CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale=CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh=0x0000;
    can_filter.CAN_FilterIdLow=0x0000;
    can_filter.CAN_FilterMaskIdHigh=0x0000;
    can_filter.CAN_FilterMaskIdLow=0x0000;
    can_filter.CAN_FilterFIFOAssignment=0;//the message which pass the filter save in fifo0
    can_filter.CAN_FilterActivation=ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
//    CAN_ITConfig(CAN2,CAN_IT_TME,ENABLE);
}

void CAN2_TX_IRQHandler(void) //CAN TX
{
  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)    //if transmit mailbox is empty 
  {
	   CAN_ClearITPendingBit(CAN2,CAN_IT_TME);   
  }
//	CAN_ClearITPendingBit(CAN2, CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_LEC|CAN_IT_ERR);

}

void CAN2_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
    {
        CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);//|CAN_IT_FF0|CAN_IT_FOV0
        CAN_Receive(CAN2, CAN_FIFO0, &rx_message);  
       //������������ݴ���
       
				CAN2_Data_Receive_Process
//			CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
    }
		
//		CAN_ClearITPendingBit(CAN2, CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_LEC|CAN_IT_ERR);
}
#endif
