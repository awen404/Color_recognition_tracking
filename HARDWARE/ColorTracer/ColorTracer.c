#include "ColorTracer.h"
#include "lcd.h"
#include "led.h"
#include "beep.h"
#include "usart.h"

RESULT result;

//TARGET_CONDITION condition={50,80,20,250,20,200,40,40,320,240};

//ʶ�������ɫ
TARGET_CONDITION condition0={
	60,				 //Ŀ����Сɫ�ȣ�H_MIN
	110,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	45,        //Ŀ����С���Ͷȣ�S_MIN
	110,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	90,        //Ŀ����С���ȣ�L_MIN
	210,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
//��ɫ
TARGET_CONDITION condition1={
	110,				 //Ŀ����Сɫ�ȣ�H_MIN
	150,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	50,        //Ŀ����С���Ͷȣ�S_MIN
	150,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	100,        //Ŀ����С���ȣ�L_MIN
	200,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};

//��ɫ
TARGET_CONDITION condition2={
	160,				 //Ŀ����Сɫ�ȣ�H_MIN
	190,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	15,        //Ŀ����С���Ͷȣ�S_MIN
	100,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	15,        //Ŀ����С���ȣ�L_MIN
	190,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
//��ɫ
TARGET_CONDITION condition3={
	80,				 //Ŀ����Сɫ�ȣ�H_MIN
	160,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	10,        //Ŀ����С���Ͷȣ�S_MIN
	100,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	10,        //Ŀ����С���ȣ�L_MIN
	100,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
//��ɫ
TARGET_CONDITION condition4={
	1,				 //Ŀ����Сɫ�ȣ�H_MIN
	40,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	50,        //Ŀ����С���Ͷȣ�S_MIN
	120,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	40,        //Ŀ����С���ȣ�L_MIN
	220,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX	
};


//��ɫƹ����,�Լ��û�ͼ�������  --����
TARGET_CONDITION condition5={
	20,				 //Ŀ����Сɫ�ȣ�H_MIN
	60,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	30,        //Ŀ����С���Ͷȣ�S_MIN
	240,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	60,        //Ŀ����С���ȣ�L_MIN
	240,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	 
	240,       //Ŀ������ȣ�WIDTH_MAX   ����
	320        //Ŀ�����߶ȣ�HEIGHT_MAX		����
};

//��ɫ
TARGET_CONDITION condition6={
	7,				 //Ŀ����Сɫ�ȣ�H_MIN
	22,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	1,        //Ŀ����С���Ͷȣ�S_MIN
	55,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	1,        //Ŀ����С���ȣ�L_MIN
	160,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
//�Ұ�
TARGET_CONDITION condition7={
	60,				 //Ŀ����Сɫ�ȣ�H_MIN
	170,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	0,        //Ŀ����С���Ͷȣ�S_MIN
	25,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	160,        //Ŀ����С���ȣ�L_MIN
	220,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
//��ɫ
TARGET_CONDITION condition8={
	200,				 //Ŀ����Сɫ�ȣ�H_MIN
	240,       //Ŀ�����ɫ�ȣ�H_MAX
	           
	5,        //Ŀ����С���Ͷȣ�S_MIN
	240,       //Ŀ����󱥺Ͷȣ�S_MAX
	           
	5,        //Ŀ����С���ȣ�L_MIN
	240,       //Ŀ��������ȣ�L_MAX
	           
	40,        //Ŀ����С��ȣ�WIDTH_MIN
	40,        //Ŀ����С�߶ȣ�HEIGHT_MIN
	           
	240,       //Ŀ������ȣ�WIDTH_MAX
	320        //Ŀ�����߶ȣ�HEIGHT_MAX
};
#define minOf3Values( v1, v2, v3 )			( (v1<v2) ? ( (v1<v3) ? (v1) : (v3) ) : ( (v2<v3) ? (v2) : (v3) ) )//ȡrgb�е���Сֵ

#define maxOf3Values( v1, v2, v3 )			( (v1>v2) ? ( (v1>v3) ? (v1) : (v3) ) : ( (v2>v3) ? (v2) : (v3) ) )//ȡrgb�е����ֵ

typedef struct 						//RGB
{
	unsigned char Red;					// [0,255]
	unsigned char Green;        // [0,255]
	unsigned char Blue;         // [0,255]
}COLOR_RGB;

typedef struct						//HLS��ɫ
{
	unsigned char Hue;					//ɫ��	,[0,240]					     
	unsigned char Saturation;		//���Ͷ�,[0,240]	
	unsigned char Lightness;		//����	,[0,240]
}COLOR_HLS;

COLOR_HLS h;
typedef struct						//��Ѱ����
{
	unsigned int X_Start;
	unsigned int X_End;
	unsigned int Y_Start;
	unsigned int Y_End;
}SEARCH_AREA;


/**
 * @brief  ��ȡ ILI9341 ��ʾ����ĳһ����������������
 * @param  usX �����ض�ɨ�跽���¸õ��X����
 * @param  usY �����ض�ɨ�跽���¸õ��Y����
 * @retval ��������
 */
//uint16_t ILI9341_GetPointPixel( uint16_t usX, uint16_t usY )

/**
 * @brief  ��ȡĳһ����ɫ����
 * @param  usX ���õ��X����
 * @param  usY ���õ��Y����
 * @param  color_rgb ��COLOR_RGB�ṹ�壬�洢��ɫ����
 * @retval ��
 */
static void ReadColor( uint16_t usX, uint16_t usY, COLOR_RGB* color_rgb )
{
	unsigned short rgb;
	
	rgb = LCD_ReadPoint( usX, usY );					//��ȡ��ɫ����
	
	//ת����ֵ��Ϊ[0,255]����ԭɫֵ
	color_rgb->Red 		= (unsigned char)( ( rgb & 0xF800 ) >> 8 );
	color_rgb->Green    = (unsigned char)( ( rgb & 0x07E0 ) >> 3 );
	color_rgb->Blue 	= (unsigned char)( ( rgb & 0x001F ) << 3 );
}

/**
 * @brief  RGBתHLS
 * @param  color_rgb ��COLOR_RGB�ṹ�壬�洢RGB��ʽ��ɫ����
 * @param  color_hls ��COLOR_HLS�ṹ�壬�洢HLS��ʽ��ɫ����
 * @retval ��
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
	
	//����l������
	l = ( max + min ) * 240 / 255 / 2;
	
	//����h��ɫ��
	if( max == min )//�޶���
	{
		s = 0;
		h = 0;
	}
	else
	{
		//����ɫ��
		if( max == r )//���ֵΪ��ɫ
		{ 
//			if( min == b )//h����0��40  ��СֵΪ��ɫ
//			{
//				h = 40 * ( g - b ) / dif;//h = 40 * ( g - n ) / dif;
//			}
//			else if( min == g )//h����200��240 ��СֵΪ��ɫ
//			{
//				h = 40 * ( g - b ) / dif + 240;
//			}
			
			if( g>=b )//h����0��40  ��СֵΪ��ɫ
			{
				h = 40 * ( g - b ) / dif;//h = 40 * ( g - n ) / dif;
			}
			else //h����200��240 ��СֵΪ��ɫ
			{
				h = 40 * ( g - b ) / dif + 240;
			}
		}
		else if( max == g )//���ֵΪ��ɫ 40��120
		{
			h = 40 * ( b - r ) / dif + 80;
		}
		else if( max == b )//���ֵΪ��ɫ 120 -200
		{
			h = 40 * ( r - g ) / dif + 160;
		}
		
		//���㱥�Ͷ�
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
			s = (dif) * 240 / ( 511 - ( max + min ) );	//����¥����˵����480��Ϊ511
		}
		
	}
//	H = h;
//	S = s;
//	L = l;
	color_hls->Hue = (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));					//ɫ��
	color_hls->Saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));	//���Ͷ�
	color_hls->Lightness = (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));		//����
	
}

/**
 * @brief  ��ɫƥ��
 * @param  color_hls ��COLOR_HLS�ṹ�壬�洢HLS��ʽ��ɫ����
 * @param  condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
 * @retval 1�����ص���ɫ��Ŀ�귶Χ�ڣ�0�����ص���ɫ����Ŀ�귶Χ�ڡ�
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
 * @brief  Ѱ�Ҹ�ʴ����
 * @param  x ����ʴ����x����
 * @param  y ����ʴ����y����
 * @param  condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
 * @param  area ��SEARCH_AREA�ṹ�壬���Ҹ�ʴ���ĵ�����
 * @retval 1���ҵ��˸�ʴ���ģ�x��yΪ��ʴ���ĵ����ꣻ0��û���ҵ���ʴ���ġ�
 */
static int SearchCenter(unsigned int* x, unsigned int* y, const TARGET_CONDITION* condition, SEARCH_AREA* area )
{
	unsigned int i, j, k;
	unsigned int FailCount=0;
	unsigned int SpaceX, SpaceY;
	COLOR_RGB rgb;
	COLOR_HLS hls;
	
	SpaceX = condition->WIDTH_MIN / 3;//��С��� 40/3
	SpaceY = condition->HEIGHT_MIN / 3;//��С�߶� 40/3
	
	for(i=area->Y_Start; i<area->Y_End; i+=SpaceY)		//Y_Start,Y_End��Trace()�����б�����
	{
		for(j=area->X_Start; j<area->X_End; j+=SpaceX)
		{
			FailCount = 0;
			for(k=0; k<SpaceX+SpaceY; k++)		//���ļ�����ͼ1��ʾ
			{
				if(k<SpaceX)
					ReadColor( j+k, i+SpaceY/2, &rgb );
				else
					ReadColor( j+SpaceX/2, i+(k-SpaceX), &rgb );
				RGB2HSL( &rgb, &hls );
				
				if(!ColorMatch( &hls, condition ))
					FailCount++;
				
				if(FailCount>( (SpaceX+SpaceY) >> ALLOW_FAIL_PER ))		//�����ƥ���������(��������/8),��break�˴�ѭ��
					break;																							//������ֵΪ0
				
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
 * @brief  �Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
 * @param  oldX ����ǰ�ĸ�ʴ����x����
 * @param  oldY ����ǰ�ĸ�ʴ����y����
 * @param  condition ��TARGET_CONDITION�ṹ�壬���ϣ������ɫ������ֵ
 * @param  result ��RESULT�ṹ�壬��ż����
 * @retval 1�����ɹ���0�����ʧ�ܡ�
 */
static int Corrode(unsigned int oldX, unsigned int oldY, const TARGET_CONDITION* condition, RESULT* result )
{
	unsigned int Xmin, Xmax, Ymin, Ymax;
	unsigned int i;
	unsigned int FailCount=0;
	COLOR_RGB rgb;
	COLOR_HLS hls;
	//�����ĵ�鵽x�����
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
	//�����ĵ�鵽x���Ҳ�
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
	//y��
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
	//y��
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


//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������1ʶ��ʧ��
int Trace(const TARGET_CONDITION* condition, RESULT* result_final, MOVE *move)
{
	unsigned int i;
	static unsigned int x0, y0, Flag = 0;
	static SEARCH_AREA area = {IMG_X, IMG_X+IMG_W, IMG_Y, IMG_Y+IMG_H};//��������
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
		Corrode(result.x, result.y, condition, &result);	//�Ӹ�ʴ�������ⸯʴ���õ��µĸ�ʴ����
		
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
		move->x = 120-result.x;				//��result.x>120,move->xΪ��,����.����ͬ��.
		move->y = 160-result.y;				//��result.y>160,move->yΪ��,����.ǰ��ͬ��.
			
		return 1;
	}
	else
	{
		Flag = 0;
		return 0;
	}
	
}


