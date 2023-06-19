#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "timer.h"
#include "exti.h"
#include "ColorTracer.h"
#include "key.h" 
#include "led.h" 
#include "ov7725.h"
#include "vehicle.h"

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		240 // <=240 横屏
#define  OV7725_WINDOW_HEIGHT		320 // <=320

//const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5种光照模式	    
//const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义	
extern volatile uint8_t Ov7670_Vsync;
u8 R,G,B;

//更新LCD显示(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	if(ov_sta)//有帧中断更新
	{
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
//		LCD_Scan_Dir(U2D_R2L);			//从上到下,从右到左
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7725_RRST=0;				//开始复位读指针 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束 
		OV7725_RCK_H; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H; 
				LCD->LCD_RAM=color;  
			}
		}
 		ov_sta=0;					//清零帧中断标记
		ov_frame++; 
	} 
}

int linearSpeed_x,linearSpeed_y;				//最大值为PWM最大值
int32_t angularSpeed =0;
int Move_Trace(MOVE* move)
{
	int x,y;
	if((result.x <90 || result.x>150)||(result.y <120 || result.y>200)) //若物体中心在中间矩形范围内,则不移动
	{
		if(result.x <90)															//前进
		{
			x=move->x-30; 															//减去中间矩形高度的一半		
			
			if(x>=30 && x<60)        { linearSpeed_y=(float)x/10+23;}              //由于液晶屏横屏的缘故，此处屏幕x轴计算得出的是物理Y轴(向上),			                   
			else if(x>=90)          { linearSpeed_y=28;}                           //因此是linearSpeed_y.下方linearSpeed_x同理
			else        linearSpeed_y=25;
//			angularSpeed=a;
		}
		if(result.x >150)															//后退
		{
			x=move->x+30; 																
			
			if(x<=-30 && x>-60)      { linearSpeed_y=(float)x/10-23;}
			else if(x>=-90)          { linearSpeed_y=-28;}
			else       linearSpeed_y=-25;
		}
		if(result.y <120)															
		{
			y=move->y-40; 																
			
			if(y>=30 && y<60)      { linearSpeed_x=(float)y/10+33;}
            else if(y>=60 && y<100) { linearSpeed_x=(float)y/10+28;}
            else if(y>=100)         { linearSpeed_x=37; }
            else       linearSpeed_x=35;
		}
		if(result.y >200)														
		{
			y=move->y+40; 																
			
			if(y<=-30 && y>-60)  {linearSpeed_x=(float)y/10-33;}
            else if(y<=-60 && y>-100)  {linearSpeed_x=(float)y/10-28;}
			else if(y<=-100)      {linearSpeed_x=-37;}
			else      linearSpeed_x=-35;
		}				
		return 1;
	}
	linearSpeed_x=0;
	linearSpeed_y=0;
	angularSpeed=0;
	return 0;
}

u8 i=0;	
 int main(void)
 {
	MOVE move={160,120};    //120 160？
	 
	u8 lightmode=0,saturation=0,brightness=0,contrast=0,effect=0;	
	u8 key;
	 
	delay_init();	    		  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200	
	LCD_Init();			   	
	LCD_Display_Dir(1);		//设置为横屏	
	KEY_Init();				
	LED_Init();
	TIM2_3_PWM_Init(99,719);   // 72M/720=100kHZ,计数到100时运行0.01ms
	Vehicle_Init();			//初始化小车
	
	LCD_ShowString(30,20,200,16,16,"ELITE STM32F103 ^_^");	
	LCD_ShowString(30,40,200,16,16,"OV7725 TEST");	
	LCD_ShowString(30,60,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,80,200,16,16,"2020/7/28"); 
	LCD_ShowString(30,100,200,16,16,"KEY0:QVGA");
	LCD_ShowString(30,120,200,16,16,"KEY1:VGA");
	LCD_ShowString(30,170,200,16,16,"TPAD:Effects");	 
    LCD_ShowString(30,190,200,16,16,"OV7725 Init...");	 
	 

	while (OV7725_Init())
	{
			LCD_ShowString(30,190,200,16,16,"OV7725_OV7670 Error!!");
			delay_ms(200);
			LCD_Fill(30,190,239,246,WHITE);
			delay_ms(200);			
	}
	LCD_ShowString(30,190,200,16,16,"OV7725 Init OK       ");
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGA模式输出
			break;
		}else if(key==KEY1_PRES)
		{
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGA模式输出
			break;
		}
		i++;
		if(i==100)LCD_ShowString(30,230,210,16,16,"KEY0:QVGA  KEY1:VGA"); //闪烁显示提示信息
		if(i==200)
		{	
			LCD_Fill(30,230,210,250+16,WHITE);
			i=0; 
		}
		delay_ms(5);	
	}
	OV7725_Light_Mode(lightmode);
	OV7725_Color_Saturation(saturation);
	OV7725_Brightness(brightness);
	OV7725_Contrast(contrast);
	OV7725_Special_Effects(effect);
	OV7725_CS=0;
	
	TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断									  
	EXTI8_Init();						//使能定时器捕获
	OV7725_Window_Set(240,320,1);	//设置窗口 --横屏
    OV7725_CS=0;					
	LCD_Clear(BLACK);
	
 	while(1)
	{	
		OV7725_camera_refresh();//更新显示
        
		if(Ov7670_Vsync == 2)
		{
			Ov7670_Vsync = 0;		
			
			if(Trace(&condition5, &result,&move))
			{
				LCD_DrawRectangle ( result.x-result.w/2, result.y-result.h/2, result.x+result.w/2, result.y+result.h/2);	
				Move_Trace(&move);
				mecanumRun(-linearSpeed_x,linearSpeed_y,angularSpeed);
			}         
		}	
	}	   
}













