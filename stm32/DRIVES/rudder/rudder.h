
#ifndef __RUDDER_H
#define __RUDDER_H
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"	

extern int volatile DG_n; //定义舵机转动的位置，左2600，中1650，右700
extern int volatile DG_check;//检测是否需要转动舵机

void rudder_init(void);//初始化舵机

void rudder_change(void);//改变舵机角度

 #endif