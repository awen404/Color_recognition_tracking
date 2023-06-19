#include "exti.h"
#include "delay.h" 
#include "ov7725.h"
volatile uint8_t Ov7670_Vsync;
u8 ov_sta;	//帧中断标记
 //外部中断5~9服务程序
void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line8)==SET)	//是8线的中断
	{ 
		if(Ov7670_Vsync == 0)
		{
			OV7725_WRST=0;	//复位写指针		  		 
			OV7725_WRST=1;	
			OV7725_WREN=1;	//允许写入FIFO
			Ov7670_Vsync = 1;
			//ov_sta++;		//帧中断加1 			
		}else if(Ov7670_Vsync == 1)
		{										/////有改动
			OV7725_WREN=0;
			Ov7670_Vsync = 2;
			ov_sta++;
		}		 
		//ov_sta++;		//帧中断加1 
	}
	EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位						  
} 
//外部中断8初始化
void EXTI8_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);//PA8对中断线8
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器   
}











