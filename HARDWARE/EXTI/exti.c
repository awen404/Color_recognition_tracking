#include "exti.h"
#include "delay.h" 
#include "ov7725.h"
volatile uint8_t Ov7670_Vsync;
u8 ov_sta;	//֡�жϱ��
 //�ⲿ�ж�5~9�������
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//��8�ߵ��ж�
	{ 
		if(Ov7670_Vsync == 0)
		{
			OV7725_WRST=0;	//��λдָ��		  		 
			OV7725_WRST=1;	
			OV7725_WREN=1;	//����д��FIFO
			Ov7670_Vsync = 1;
			//ov_sta++;		//֡�жϼ�1 			
		}else if(Ov7670_Vsync == 1)
		{										/////�иĶ�
			OV7725_WREN=0;
			Ov7670_Vsync = 2;
			ov_sta++;
		}		 
		//ov_sta++;		//֡�жϼ�1 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI8��·����λ						  
} 
//�ⲿ�ж�8��ʼ��
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8���ж���8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���   
}











