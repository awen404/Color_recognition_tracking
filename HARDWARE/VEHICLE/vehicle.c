#include "vehicle.h"
#include "sys.h"
#include "timer.h"
#include "stdlib.h"
#include "usart.h"


int maxLinearSpeed = 100;		//最大线速度为PWM最大值100,因为没编码器，故无法测速
unsigned int pwm_data1,pwm_data2,pwm_data3,pwm_data4;

//const float L_1=0.058,W_1=0.067,R_1=0.03;         //车尺寸,前后为L，左右为W,长宽已经除2,单位m
const float L_1=0.0389,W_1=0.045,R_1=0.0451;         //车尺寸,前后为L，左右为W,长宽已经除2,单位m

int X_speed=0; 

int Y_speed=0; 
int W_speed=0;

void Vehicle_Init(void)
{
 
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟
		//初始化四个电机的八个控制端IO
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOF, &GPIO_InitStructure);					
	
	 GPIO_ResetBits(GPIOF,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);//初始化四个电机的控制端口为00，制动

}

//设置电机PWM
void setEachMotorSpeed(unsigned int pwm_data1, unsigned int pwm_data2, unsigned int pwm_data3, unsigned int pwm_data4)
{
	TIM_SetCompare1(TIM3,pwm_data1);//左1
	TIM_SetCompare2(TIM2,pwm_data2);//左2
	TIM_SetCompare3(TIM2,pwm_data3);//右1
	TIM_SetCompare4(TIM2,pwm_data4);//右2
}

//X_speed:X轴速度
//Y_speed:Y轴速度
//W_speed:角速度
void mecanumRun(float X_speed, float Y_speed, float W_speed)
{
	unsigned int max;
	float speed1,speed2,speed3,speed4;

	speed1 = (Y_speed + X_speed - (W_1+L_1)*W_speed/R_1);//根据逆运动学方程式算出
	speed2 = (Y_speed - X_speed - (W_1+L_1)*W_speed/R_1);       
	speed3 = (Y_speed - X_speed + (W_1+L_1)*W_speed/R_1);
	speed4 = (Y_speed + X_speed + (W_1+L_1)*W_speed/R_1);
	
	if(speed1 > 0)Left1_moto_go  else Left1_moto_back	//控制轮子正反转
	if(speed2 > 0)Left2_moto_go  else Left2_moto_back
	if(speed3 > 0)Right1_moto_go  else Right1_moto_back
	if(speed4 > 0)Right2_moto_go  else Right2_moto_back
		
	pwm_data1=abs(speed1);
	pwm_data2=abs(speed2);
	pwm_data3=abs(speed3);
	pwm_data4=abs(speed4);
	
	max = pwm_data1;
	if (max < pwm_data2)   max = pwm_data2;
	if (max < pwm_data3)   max = pwm_data3;
	if (max < pwm_data4)   max = pwm_data4;	
	
	if (max > maxLinearSpeed)		//等比改变四个轮子的PWM值
	{
			pwm_data1 =(float) pwm_data1 / max * maxLinearSpeed;
			pwm_data2 =(float) pwm_data2 / max * maxLinearSpeed;
			pwm_data3 =(float) pwm_data3 / max * maxLinearSpeed;
			pwm_data4 =(float) pwm_data4 / max * maxLinearSpeed;
	}
	
	setEachMotorSpeed(pwm_data1, pwm_data2, pwm_data3, pwm_data4);
}

void go_forward(void)
{
	Left1_moto_go;
	Left2_moto_go;
	
	Right1_moto_go;
	Right2_moto_go;
}

void go_backwards(void)
{
	Left1_moto_back;
	Left2_moto_back;
	
	Right1_moto_back;
	Right2_moto_back;
}
