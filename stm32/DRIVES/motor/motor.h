

#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usart.h"

//���ڵĶ��壬ż��
//����ƽ��Ĳ�ֵ,before_pwmBӦ�ü�ȥ��ֵ
#define TIM3_period (1000-1)
#define TIM1_period (1000-1)
#define BLance 0


 void motor_config(void);//������ó�ʼ��
 void motor_rcc_config(void);//��ʼ��ʱ��
 void motor_gpio_config(void);//��ʼ������
 void motor_tim_config(void);//��ʼ��TIM3,TIM1��ʱ��
 
 //���ĵ������������
 void motor_direction(int n);//����ǰ�����Ǻ���,0����ǰ����1�������
 void motor_pwm(int n,unsigned int pwm);//��ʹ�ܶ˵�pwm,0����ǰA,1����ǰB��2�����A,3�����B
 
 
 
 //ʹ���ĺ��������Ч
 void motor_gpio_change(void);//ʹת����Ч
 void motor_tim_change(void);//ʹTIM3,TIM1��ʱ������������Ч
 #endif
 