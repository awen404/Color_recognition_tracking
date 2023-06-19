#ifndef _VEHICLE_H
#define _VEHICLE_H
#include "sys.h"


//定义小车电机驱动模块输出IO口
#define LOUT1_0  PFout(6) //左1电机INT1
#define LOUT1_1  PFout(7) //左1电机INT2
#define LOUT2_0  PFout(4) //左2 INT3
#define LOUT2_1  PFout(5) //左2 INT4
#define ROUT1_0	 PFout(13) //右1 INT3
#define ROUT1_1	 PFout(12) //右1 INT4
#define ROUT2_0	 PFout(14) //右2 INT1
#define ROUT2_1	 PFout(15) //右2 INT2


//定义电机转动方向
#define Left1_moto_go      {LOUT1_0 = 0,LOUT1_1 = 1;}   //左1电机正转
#define Left1_moto_back    {LOUT1_0 = 1,LOUT1_1 = 0;}   //左1电机反转
#define Left1_moto_stop    {LOUT1_0 = 0,LOUT1_1 = 0;}   //制动

#define Left2_moto_go      {LOUT2_0 = 0,LOUT2_1 = 1;}   //左2电机正转
#define Left2_moto_back    {LOUT2_0 = 1,LOUT2_1 = 0;}   //左2电机反转
#define Left2_moto_stop    {LOUT2_0 = 0,LOUT2_1 = 0;}   //制动
                     
#define Right1_moto_go     {ROUT1_0 = 0,ROUT1_1 = 1;}   //右1 正转
#define Right1_moto_back   {ROUT1_0 = 1,ROUT1_1 = 0;}   //右1 反转
#define Right1_moto_stop   {ROUT1_0 = 0,ROUT1_1 = 0;}   //制动

#define Right2_moto_go     {ROUT2_0 = 0,ROUT2_1 = 1;}   //右2 正转
#define Right2_moto_back   {ROUT2_0 = 1,ROUT2_1 = 0;}   //右2 反转
#define Right2_moto_stop   {ROUT2_0 = 0,ROUT2_1 = 0;}   //制动


void Vehicle_Init(void);
void mecanumRun(float X_speed, float Y_speed, float W_speed);
void setEachMotorSpeed(unsigned int pwm_data1, unsigned int pwm_data2, unsigned int pwm_data3, unsigned int pwm_data4);	

void go_forward(void);
void go_backwards(void);


#endif
