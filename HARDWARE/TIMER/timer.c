#include "timer.h"
#include "usart.h"

//PA6:TIM3_CH1  左1电机使能位

//PA1:TIM2_CH2  左2电机使能位
//PA2:TIM2_CH3  右1电机使能位
//PA3:TIM2_CH4  右2电机使能位

//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM2_3_PWM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	
/********************GPIO初始化*****************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 |RCC_APB1Periph_TIM3, ENABLE);	//使能定时器2与定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
 
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  						//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_6 |GPIO_Pin_1; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);										
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_6 |GPIO_Pin_1);	//PA 输出低,电机不使能
	
	
	
/********************PWM及TIM的初始化*****************/	
   //初始化TIM2,3
	TIM_TimeBaseStructure.TIM_Period = arr; 	//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	
	//初始化TIM2,TIM3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 						//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 			//输出极性:TIM输出比较极性低
	
//---------------------初始化单个通道---------------------
//初始化PA6-TIM3_CH1->左1电机使能位  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM3 OC1
	
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   //使能TIM3在CCR1上的预装载寄存器
	
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3


//初始化PA2-TIM2_CH3->右1电机使能位  PA3-TIM2_CH4->右2电机使能位  PA1-TIM2_CH2->左2电机使能位
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM2 OC2
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM2 OC3
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM2 OC4
	
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);   //使能TIM2在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   //使能TIM2在CCR3上的预装载寄存器
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);   //使能TIM2在CCR4上的预装载寄存器
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2

//配置中断3的优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 				 //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
//配置中断2的优先级。设置两个中断先占、从优先级相同，不能互相打断
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 					  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  				//从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 						//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}

//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		}
}


u8 ov_frame; 	//统计帧数
//定时器6中断服务程序	 
void TIM6_IRQHandler(void)
{ 		    		  			    
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{				    
		//printf("frame:%dfps\r\n",ov_frame);	//打印帧率
		ov_frame=0;		    				   				     	    	
	}				   
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 	    
}
//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断
 
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 									 
}


