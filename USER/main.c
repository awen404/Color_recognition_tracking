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

//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		240 // <=240 ����
#define  OV7725_WINDOW_HEIGHT		320 // <=320

//const u8*LMODE_TBL[5]={"Auto","Sunny","Cloudy","Office","Home"};							//5�ֹ���ģʽ	    
//const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 
extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��	
extern volatile uint8_t Ov7670_Vsync;
u8 R,G,B;

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void)
{
	u32 i,j;
 	u16 color;	 
	if(ov_sta)//��֡�жϸ���
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������
//		LCD_Scan_Dir(U2D_R2L);			//���ϵ���,���ҵ���
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7725_RRST=0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ����� 
		OV7725_RCK_H; 
		for(i=0;i<OV7725_WINDOW_HEIGHT;i++)
		{
			for(j=0;j<OV7725_WINDOW_WIDTH;j++)
			{
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				color<<=8;  
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H; 
				LCD->LCD_RAM=color;  
			}
		}
 		ov_sta=0;					//����֡�жϱ��
		ov_frame++; 
	} 
}

int linearSpeed_x,linearSpeed_y;				//���ֵΪPWM���ֵ
int32_t angularSpeed =0;
int Move_Trace(MOVE* move)
{
	int x,y;
	if((result.x <90 || result.x>150)||(result.y <120 || result.y>200)) //�������������м���η�Χ��,���ƶ�
	{
		if(result.x <90)															//ǰ��
		{
			x=move->x-30; 															//��ȥ�м���θ߶ȵ�һ��		
			
			if(x>=30 && x<60)        { linearSpeed_y=(float)x/10+23;}              //����Һ����������Ե�ʣ��˴���Ļx�����ó���������Y��(����),			                   
			else if(x>=90)          { linearSpeed_y=28;}                           //�����linearSpeed_y.�·�linearSpeed_xͬ��
			else        linearSpeed_y=25;
//			angularSpeed=a;
		}
		if(result.x >150)															//����
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
	MOVE move={160,120};    //120 160��
	 
	u8 lightmode=0,saturation=0,brightness=0,contrast=0,effect=0;	
	u8 key;
	 
	delay_init();	    		  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200	
	LCD_Init();			   	
	LCD_Display_Dir(1);		//����Ϊ����	
	KEY_Init();				
	LED_Init();
	TIM2_3_PWM_Init(99,719);   // 72M/720=100kHZ,������100ʱ����0.01ms
	Vehicle_Init();			//��ʼ��С��
	
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
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���
			break;
		}else if(key==KEY1_PRES)
		{
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGAģʽ���
			break;
		}
		i++;
		if(i==100)LCD_ShowString(30,230,210,16,16,"KEY0:QVGA  KEY1:VGA"); //��˸��ʾ��ʾ��Ϣ
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
	
	TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�									  
	EXTI8_Init();						//ʹ�ܶ�ʱ������
	OV7725_Window_Set(240,320,1);	//���ô��� --����
    OV7725_CS=0;					
	LCD_Clear(BLACK);
	
 	while(1)
	{	
		OV7725_camera_refresh();//������ʾ
        
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













