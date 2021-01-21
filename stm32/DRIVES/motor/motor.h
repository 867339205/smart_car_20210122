

#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "usart.h"

//周期的定义，偶数
//定义平衡的差值,before_pwmB应该减去的值
#define TIM3_period (1000-1)
#define TIM1_period (1000-1)
#define BLance 0


 void motor_config(void);//电机配置初始化
 void motor_rcc_config(void);//初始化时钟
 void motor_gpio_config(void);//初始化引脚
 void motor_tim_config(void);//初始化TIM3,TIM1定时器
 
 //更改电机驱动的配置
 void motor_direction(int n);//车子前进还是后退,0代表前进，1代表后退
 void motor_pwm(int n,unsigned int pwm);//改使能端的pwm,0代表前A,1代表前B，2代表后A,3代表后B
 
 
 
 //使更改后的配置生效
 void motor_gpio_change(void);//使转向生效
 void motor_tim_change(void);//使TIM3,TIM1定时器的脉冲宽度生效
 #endif
 