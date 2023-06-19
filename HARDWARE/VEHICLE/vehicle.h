#ifndef _VEHICLE_H
#define _VEHICLE_H
#include "sys.h"


//����С���������ģ�����IO��
#define LOUT1_0  PFout(6) //��1���INT1
#define LOUT1_1  PFout(7) //��1���INT2
#define LOUT2_0  PFout(4) //��2 INT3
#define LOUT2_1  PFout(5) //��2 INT4
#define ROUT1_0	 PFout(13) //��1 INT3
#define ROUT1_1	 PFout(12) //��1 INT4
#define ROUT2_0	 PFout(14) //��2 INT1
#define ROUT2_1	 PFout(15) //��2 INT2


//������ת������
#define Left1_moto_go      {LOUT1_0 = 0,LOUT1_1 = 1;}   //��1�����ת
#define Left1_moto_back    {LOUT1_0 = 1,LOUT1_1 = 0;}   //��1�����ת
#define Left1_moto_stop    {LOUT1_0 = 0,LOUT1_1 = 0;}   //�ƶ�

#define Left2_moto_go      {LOUT2_0 = 0,LOUT2_1 = 1;}   //��2�����ת
#define Left2_moto_back    {LOUT2_0 = 1,LOUT2_1 = 0;}   //��2�����ת
#define Left2_moto_stop    {LOUT2_0 = 0,LOUT2_1 = 0;}   //�ƶ�
                     
#define Right1_moto_go     {ROUT1_0 = 0,ROUT1_1 = 1;}   //��1 ��ת
#define Right1_moto_back   {ROUT1_0 = 1,ROUT1_1 = 0;}   //��1 ��ת
#define Right1_moto_stop   {ROUT1_0 = 0,ROUT1_1 = 0;}   //�ƶ�

#define Right2_moto_go     {ROUT2_0 = 0,ROUT2_1 = 1;}   //��2 ��ת
#define Right2_moto_back   {ROUT2_0 = 1,ROUT2_1 = 0;}   //��2 ��ת
#define Right2_moto_stop   {ROUT2_0 = 0,ROUT2_1 = 0;}   //�ƶ�


void Vehicle_Init(void);
void mecanumRun(float X_speed, float Y_speed, float W_speed);
void setEachMotorSpeed(unsigned int pwm_data1, unsigned int pwm_data2, unsigned int pwm_data3, unsigned int pwm_data4);	

void go_forward(void);
void go_backwards(void);


#endif
