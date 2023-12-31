#include "ColorTracer.h"
#include "lcd.h"
#include "led.h"
#include "beep.h"
#include "usart.h"

RESULT result;

//TARGET_CONDITION condition={50,80,20,250,20,200,40,40,320,240};

//识别的是绿色
TARGET_CONDITION condition0={
	60,				 //目标最小色度，H_MIN
	110,       //目标最大色度，H_MAX
	           
	45,        //目标最小饱和度，S_MIN
	110,       //目标最大饱和度，S_MAX
	           
	90,        //目标最小亮度，L_MIN
	210,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
//蓝色
TARGET_CONDITION condition1={
	110,				 //目标最小色度，H_MIN
	150,       //目标最大色度，H_MAX
	           
	50,        //目标最小饱和度，S_MIN
	150,       //目标最大饱和度，S_MAX
	           
	100,        //目标最小亮度，L_MIN
	200,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};

//紫色
TARGET_CONDITION condition2={
	160,				 //目标最小色度，H_MIN
	190,       //目标最大色度，H_MAX
	           
	15,        //目标最小饱和度，S_MIN
	100,       //目标最大饱和度，S_MAX
	           
	15,        //目标最小亮度，L_MIN
	190,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
//黑色
TARGET_CONDITION condition3={
	80,				 //目标最小色度，H_MIN
	160,       //目标最大色度，H_MAX
	           
	10,        //目标最小饱和度，S_MIN
	100,       //目标最大饱和度，S_MAX
	           
	10,        //目标最小亮度，L_MIN
	100,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
//橙色
TARGET_CONDITION condition4={
	1,				 //目标最小色度，H_MIN
	40,       //目标最大色度，H_MAX
	           
	50,        //目标最小饱和度，S_MIN
	120,       //目标最大饱和度，S_MAX
	           
	40,        //目标最小亮度，L_MIN
	220,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX	
};


//黄色乒乓球,自己用绘图测出来的  --横屏
TARGET_CONDITION condition5={
	20,				 //目标最小色度，H_MIN
	60,       //目标最大色度，H_MAX
	           
	30,        //目标最小饱和度，S_MIN
	240,       //目标最大饱和度，S_MAX
	           
	60,        //目标最小亮度，L_MIN
	240,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	 
	240,       //目标最大宽度，WIDTH_MAX   横屏
	320        //目标最大高度，HEIGHT_MAX		横屏
};

//棕色
TARGET_CONDITION condition6={
	7,				 //目标最小色度，H_MIN
	22,       //目标最大色度，H_MAX
	           
	1,        //目标最小饱和度，S_MIN
	55,       //目标最大饱和度，S_MAX
	           
	1,        //目标最小亮度，L_MIN
	160,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
//灰白
TARGET_CONDITION condition7={
	60,				 //目标最小色度，H_MIN
	170,       //目标最大色度，H_MAX
	           
	0,        //目标最小饱和度，S_MIN
	25,       //目标最大饱和度，S_MAX
	           
	160,        //目标最小亮度，L_MIN
	220,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
//红色
TARGET_CONDITION condition8={
	200,				 //目标最小色度，H_MIN
	240,       //目标最大色度，H_MAX
	           
	5,        //目标最小饱和度，S_MIN
	240,       //目标最大饱和度，S_MAX
	           
	5,        //目标最小亮度，L_MIN
	240,       //目标最大亮度，L_MAX
	           
	40,        //目标最小宽度，WIDTH_MIN
	40,        //目标最小高度，HEIGHT_MIN
	           
	240,       //目标最大宽度，WIDTH_MAX
	320        //目标最大高度，HEIGHT_MAX
};
#define minOf3Values( v1, v2, v3 )			( (v1<v2) ? ( (v1<v3) ? (v1) : (v3) ) : ( (v2<v3) ? (v2) : (v3) ) )//取rgb中的最小值

#define maxOf3Values( v1, v2, v3 )			( (v1>v2) ? ( (v1>v3) ? (v1) : (v3) ) : ( (v2>v3) ? (v2) : (v3) ) )//取rgb中的最大值

typedef struct 						//RGB
{
	unsigned char Red;					// [0,255]
	unsigned char Green;        // [0,255]
	unsigned char Blue;         // [0,255]
}COLOR_RGB;

typedef struct						//HLS颜色
{
	unsigned char Hue;					//色度	,[0,240]					     
	unsigned char Saturation;		//饱和度,[0,240]	
	unsigned char Lightness;		//亮度	,[0,240]
}COLOR_HLS;

COLOR_HLS h;
typedef struct						//搜寻区域
{
	unsigned int X_Start;
	unsigned int X_End;
	unsigned int Y_Start;
	unsigned int Y_End;
}SEARCH_AREA;


/**
 * @brief  获取 ILI9341 显示器上某一个坐标点的像素数据
 * @param  usX ：在特定扫描方向下该点的X坐标
 * @param  usY ：在特定扫描方向下该点的Y坐标
 * @retval 像素数据
 */
//uint16_t ILI9341_GetPointPixel( uint16_t usX, uint16_t usY )

/**
 * @brief  读取某一点颜色数据
 * @param  usX ：该点的X坐标
 * @param  usY ：该点的Y坐标
 * @param  color_rgb ：COLOR_RGB结构体，存储颜色数据
 * @retval 无
 */
static void ReadColor( uint16_t usX, uint16_t usY, COLOR_RGB* color_rgb )
{
	unsigned short rgb;
	
	rgb = LCD_ReadPoint( usX, usY );					//获取颜色数据
	
	//转换成值域为[0,255]的三原色值
	color_rgb->Red 		= (unsigned char)( ( rgb & 0xF800 ) >> 8 );
	color_rgb->Green    = (unsigned char)( ( rgb & 0x07E0 ) >> 3 );
	color_rgb->Blue 	= (unsigned char)( ( rgb & 0x001F ) << 3 );
}

/**
 * @brief  RGB转HLS
 * @param  color_rgb ：COLOR_RGB结构体，存储RGB格式颜色数据
 * @param  color_hls ：COLOR_HLS结构体，存储HLS格式颜色数据
 * @retval 无
 */
u8 H,S,L;
static void RGB2HSL( const COLOR_RGB* color_rgb, COLOR_HLS* color_hls )
{
	int r, g, b;
	int h, l, s;
	int max, min, dif;
	
	r = color_rgb->Red;
	g = color_rgb->Green;
	b = color_rgb->Blue;
	
	max = maxOf3Values( r, g, b );
	min = minOf3Values( r, g, b );
	dif = max - min;
	
	//计算l，亮度
	l = ( max + min ) * 240 / 255 / 2;
	
	//计算h，色度
	if( max == min )//无定义
	{
		s = 0;
		h = 0;
	}
	else
	{
		//计算色度
		if( max == r )//最大值为红色
		{ 
//			if( min == b )//h介于0到40  最小值为蓝色
//			{
//				h = 40 * ( g - b ) / dif;//h = 40 * ( g - n ) / dif;
//			}
//			else if( min == g )//h介于200到240 最小值为绿色
//			{
//				h = 40 * ( g - b ) / dif + 240;
//			}
			
			if( g>=b )//h介于0到40  最小值为蓝色
			{
				h = 40 * ( g - b ) / dif;//h = 40 * ( g - n ) / dif;
			}
			else //h介于200到240 最小值为绿色
			{
				h = 40 * ( g - b ) / dif + 240;
			}
		}
		else if( max == g )//最大值为绿色 40到120
		{
			h = 40 * ( b - r ) / dif + 80;
		}
		else if( max == b )//最大值为蓝色 120 -200
		{
			h = 40 * ( r - g ) / dif + 160;
		}
		
		//计算饱和度
		if( l == 0 )
		{
			s = 0;
		}
		else if( l <= 120 )
		{
			s = (dif) * 240 / ( max + min );
		}
		else
		{
			s = (dif) * 240 / ( 511 - ( max + min ) );	//根据楼主所说，将480改为511
		}
		
	}
//	H = h;
//	S = s;
//	L = l;
	color_hls->Hue = (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));					//色度
	color_hls->Saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));	//饱和度
	color_hls->Lightness = (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));		//亮度
	
}

/**
 * @brief  颜色匹配
 * @param  color_hls ：COLOR_HLS结构体，存储HLS格式颜色数据
 * @param  condition ：TARGET_CONDITION结构体，存放希望的颜色数据阈值
 * @retval 1：像素点颜色在目标范围内；0：像素点颜色不在目标范围内。
 */
static int ColorMatch(const COLOR_HLS* color_hls, const TARGET_CONDITION* condition )
{
	if(
			color_hls->Hue > condition->H_MIN &&
			color_hls->Hue < condition->H_MAX &&
			color_hls->Lightness > condition->L_MIN &&
			color_hls->Lightness < condition->L_MAX &&
			color_hls->Saturation > condition->S_MIN &&
			color_hls->Saturation < condition->S_MAX
	)
	{
		
		H = color_hls->Hue;
		S = color_hls->Saturation;
		L = color_hls->Lightness;return 1;
	}
		
	else
		return 0;
}

/**
 * @brief  寻找腐蚀中心
 * @param  x ：腐蚀中心x坐标
 * @param  y ：腐蚀中心y坐标
 * @param  condition ：TARGET_CONDITION结构体，存放希望的颜色数据阈值
 * @param  area ：SEARCH_AREA结构体，查找腐蚀中心的区域
 * @retval 1：找到了腐蚀中心，x、y为腐蚀中心的坐标；0：没有找到腐蚀中心。
 */
static int SearchCenter(unsigned int* x, unsigned int* y, const TARGET_CONDITION* condition, SEARCH_AREA* area )
{
	unsigned int i, j, k;
	unsigned int FailCount=0;
	unsigned int SpaceX, SpaceY;
	COLOR_RGB rgb;
	COLOR_HLS hls;
	
	SpaceX = condition->WIDTH_MIN / 3;//最小宽度 40/3
	SpaceY = condition->HEIGHT_MIN / 3;//最小高度 40/3
	
	for(i=area->Y_Start; i<area->Y_End; i+=SpaceY)		//Y_Start,Y_End在Trace()函数中被定义
	{
		for(j=area->X_Start; j<area->X_End; j+=SpaceX)
		{
			FailCount = 0;
			for(k=0; k<SpaceX+SpaceY; k++)		//如文件夹中图1所示
			{
				if(k<SpaceX)
					ReadColor( j+k, i+SpaceY/2, &rgb );
				else
					ReadColor( j+SpaceX/2, i+(k-SpaceX), &rgb );
				RGB2HSL( &rgb, &hls );
				
				if(!ColorMatch( &hls, condition ))
					FailCount++;
				
				if(FailCount>( (SpaceX+SpaceY) >> ALLOW_FAIL_PER ))		//如果不匹配次数大于(遍历次数/8),则break此次循环
					break;																							//，返回值为0
				
			}			
			if(k == SpaceX+SpaceY)
			{
				*x = j + SpaceX / 2;			
				*y = i + SpaceY / 2;
				return 1;
			}
			
		}
			
	}
	
	return 0;
		
}

/**
 * @brief  从腐蚀中心向外腐蚀，得到新的腐蚀中心
 * @param  oldX ：先前的腐蚀中心x坐标
 * @param  oldY ：先前的腐蚀中心y坐标
 * @param  condition ：TARGET_CONDITION结构体，存放希望的颜色数据阈值
 * @param  result ：RESULT结构体，存放检测结果
 * @retval 1：检测成功；0：检测失败。
 */
static int Corrode(unsigned int oldX, unsigned int oldY, const TARGET_CONDITION* condition, RESULT* result )
{
	unsigned int Xmin, Xmax, Ymin, Ymax;
	unsigned int i;
	unsigned int FailCount=0;
	COLOR_RGB rgb;
	COLOR_HLS hls;
	//从中心点查到x最左侧
	for(i=oldX; i>IMG_X; i--)
	{
		ReadColor(i, oldY, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;
		if(FailCount>(((condition->WIDTH_MIN+condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;
	}
	Xmin=i;
	//从中心点查到x最右侧
	FailCount=0;
	for(i=oldX; i<IMG_X+IMG_W; i++)
	{
		ReadColor(i, oldY, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;
		if(FailCount>(((condition->WIDTH_MIN+condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
			break;
	}
	Xmax=i;
	//y上
	FailCount=0;
	for(i=oldY; i>IMG_Y; i--)
	{
		ReadColor(oldX, i, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;
		if(FailCount>(((condition->HEIGHT_MIN+condition->HEIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;
	}
	Ymin=i;
	//y下
	FailCount=0;
	for(i=oldY; i<IMG_Y+IMG_H; i++)
	{
		ReadColor(oldX, i, &rgb);
		RGB2HSL(&rgb, &hls);
		if(!ColorMatch(&hls, condition))
			FailCount++;
		if(FailCount>(((condition->HEIGHT_MIN+condition->HEIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
			break;
	}
	Ymax=i;
	
	FailCount=0;
	
	result->x = (Xmin + Xmax) / 2;
	result->y = (Ymin + Ymax) / 2;
	result->w = (Xmax - Xmin);
	result->h = (Ymax - Ymin);
	
	if( (result->w > condition->WIDTH_MIN) && (result->w < condition->WIDTH_MAX) &&
			(result->h > condition->HEIGHT_MIN) && (result->h < condition->HEIGHT_MAX)  )
		return 1;
	else
		return 0;
}


//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDITION* condition, RESULT* result_final, MOVE *move)
{
	unsigned int i;
	static unsigned int x0, y0, Flag = 0;
	static SEARCH_AREA area = {IMG_X, IMG_X+IMG_W, IMG_Y, IMG_Y+IMG_H};//搜索区域
	RESULT result;
//	for(i = 0;i<2;i++)
//	{
		if(Flag == 0)
		{
			if(SearchCenter(&x0, &y0, condition, &area))
			{
				Flag = 1;
				//break;
			}
			else
			{
				area.X_Start = IMG_X;
				area.X_End   = IMG_X+IMG_W;
				area.Y_Start = IMG_Y;
				area.Y_End   = IMG_Y+IMG_H;
				
				if(SearchCenter(&x0, &y0, condition, &area))
				{
					Flag = 0;
					return 0;
				}
			}
		}
	//}
	
	result.x = x0;
	result.y = y0;
	
	for(i=0; i<ITERATER_NUM; i++)
	{
		Corrode(result.x, result.y, condition, &result);	//从腐蚀中心向外腐蚀，得到新的腐蚀中心
		
	}
	
	if( Corrode(result.x, result.y, condition, &result) )
	{
		x0 = result.x;
		y0 = result.y;
		result_final->x = result.x;
		result_final->y = result.y;
		result_final->w = result.w;
		result_final->h = result.h;
		Flag = 1;
		
		area.X_Start = result.x - ((result.w)>>1);
		area.X_End   = result.x + ((result.w)>>1);
		area.Y_Start = result.y - ((result.h)>>1);
		area.Y_End   = result.y + ((result.h)>>1);
		move->x = 120-result.x;				//若result.x>120,move->x为负,左移.右移同理.
		move->y = 160-result.y;				//若result.y>160,move->y为负,后退.前进同理.
			
		return 1;
	}
	else
	{
		Flag = 0;
		return 0;
	}
	
}


