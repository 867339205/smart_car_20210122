

#ifndef __INFRARED_H
#define __INFRARED_H
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "misc.h"

extern int volatile infrared_num;
extern int volatile infrared_stop;
	
 void infrared_init(void);//初始化红外模块

static void rcc_init(void);
static void gpio_init(void);
static void exti_init(void);
static void nvic_init(void);
 #endif