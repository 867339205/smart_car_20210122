
#ifndef __RUDDER_H
#define __RUDDER_H
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "delay.h"	

extern int volatile DG_n; //������ת����λ�ã���2600����1650����700
extern int volatile DG_check;//����Ƿ���Ҫת�����

void rudder_init(void);//��ʼ�����

void rudder_change(void);//�ı����Ƕ�

 #endif