#ifndef __CHE_H
#define __CHE_H	 
#include "sys.h"


//左轮  前
#define CHE_LF_M PBout(12)	// PB12  CS
#define CHE_LF_B PBout(13)	// PB13  SCK	
//左轮  后
#define CHE_LB_M PBout(14)	// PB14  MISO
#define CHE_LB_B PBout(15)	// PB15  MOSI


//右轮 前
#define CHE_RF_M PCout(10)	// PC10
#define CHE_RF_B PCout(11)	// PC11

//右轮 后

#define CHE_RB_M PCout(12)	// PC12
#define CHE_RB_B PBout(03)	// PB03


void CHE_Init(void);//初始化
void move_on();
void move_back();

void move_left();
void move_right();
void stop();
		 				    
#endif