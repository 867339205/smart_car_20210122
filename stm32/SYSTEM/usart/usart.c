/*
@file	usart.c
@brief	实现与串口通信相关的接口
*/
#include "usart.h"

static void rcc_init(void);
static void gpio_init(void);
static void nvic_init(void);


 uint8_t usart1Buf[RX_BUF_SIZE];//接收到的数据数组
 int32_t volatile isNew; //是否是新的控制数据
 int32_t volatile usart1Index; //数据的数量




/*
@brief	初始化与串口通信相关的时钟、GPIO以及串口通信的参数
@params	None
@retval	None
*/
void usart_init(void)
{
	rcc_init();
	gpio_init();
  nvic_init();
	

	USART_InitTypeDef usart_init={
		.USART_BaudRate=115200,
		.USART_WordLength=USART_WordLength_8b,
		.USART_StopBits=USART_StopBits_1,
		.USART_Parity=USART_Parity_No,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
	};
	USART_Init(USART1,&usart_init);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//启动接收中断
	//USART_ITConfig(USART2,USART_IT_TXE,ENABLE);	//启动发送中断
	
	USART_Cmd(USART1,ENABLE);
}

/*
@brief	将一个字节数据发送给上位机
@params	data 待发送的数据
@retval	None
*/
void usart_send_byte(unsigned char data){
	USART_SendData(USART1,data);
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));	//等待数据发送完成
}

/*
@brief	利用查询的方式接收一个字节数据
@params	None
@retval	接收到的一个字节数据
*/
unsigned char usart_recv_byte(void){
	while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));//查询是否接收到数据
	return USART_ReceiveData(USART1);
}

/*
@brief	初始化与串口通信相关的时钟
@params	None
@retval	None
@note	这是一个私有函数
*/
static void rcc_init(void){
	//
RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);//使能USART1,GPIOA,C时钟
}

/*
@brief	初始化与串口通信相关的GPIO
@params	None
@retval	None
@note	这是一个私有函数
*/
static void gpio_init(void){
	GPIO_InitTypeDef gpio_init;
	
	gpio_init.GPIO_Pin = GPIO_Pin_9;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpio_init);

	gpio_init.GPIO_Pin = GPIO_Pin_10;
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpio_init);
}

/*
@brief	配置串口接收中断
@params	None
@retval	None
@note	这是一个私有函数
*/
static void nvic_init(void){
	NVIC_InitTypeDef nvic_init;
	

	nvic_init.NVIC_IRQChannel=USART1_IRQn;
	nvic_init.NVIC_IRQChannelCmd=ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority=3;
	nvic_init.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&nvic_init);
}
