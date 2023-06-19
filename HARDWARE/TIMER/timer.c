#include "timer.h"
#include "usart.h"

//PA6:TIM3_CH1  ��1���ʹ��λ

//PA1:TIM2_CH2  ��2���ʹ��λ
//PA2:TIM2_CH3  ��1���ʹ��λ
//PA3:TIM2_CH4  ��2���ʹ��λ

//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM2_3_PWM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	
/********************GPIO��ʼ��*****************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��2�붨ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
 
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_6 |GPIO_Pin_1; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);										
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_6 |GPIO_Pin_1);	//PA �����,�����ʹ��
	
	
	
/********************PWM��TIM�ĳ�ʼ��*****************/	
   //��ʼ��TIM2,3
	TIM_TimeBaseStructure.TIM_Period = arr; 	//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	
	//��ʼ��TIM2,TIM3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			//�������:TIM����Ƚϼ��Ե�
	
//---------------------��ʼ������ͨ��---------------------
//��ʼ��PA6-TIM3_CH1->��1���ʹ��λ  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM3 OC1
	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   //ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
	
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3


//��ʼ��PA2-TIM2_CH3->��1���ʹ��λ  PA3-TIM2_CH4->��2���ʹ��λ  PA1-TIM2_CH2->��2���ʹ��λ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM2 OC2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM2 OC3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM2 OC4
	
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);   //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);   //ʹ��TIM2��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2

//�����ж�3�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 				 //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
//�����ж�2�����ȼ������������ж���ռ�������ȼ���ͬ�����ܻ�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 					  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure); 
 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
}


u8 ov_frame; 	//ͳ��֡��
//��ʱ��6�жϷ������	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{				    
		//printf("frame:%dfps\r\n",ov_frame);	//��ӡ֡��
		ov_frame=0;		    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 	    
}
//������ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܶ�ʱ��6���´����ж�
 
	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx����
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 									 
}


