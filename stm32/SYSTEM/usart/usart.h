/*
@file	usart.h
@brief	定义与串口通信相关的接口
*/
#ifndef __USART_H
#define __USART_H

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x.h"



#define RX_BUF_SIZE 30


extern uint8_t usart1Buf[RX_BUF_SIZE];//接收到的数据数组
extern int32_t volatile isNew; //是否是新的控制数据
extern int32_t volatile usart1Index; //数据的数量

void usart_init(void);
void usart_send_byte(unsigned char data);
unsigned char usart_recv_byte(void);

#endif
