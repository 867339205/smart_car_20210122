#include "infrared.h"


int volatile infrared_num=0;//第几次中断
int volatile infrared_stop=0;//是否要求小车停止
/*
@brief	初始化与按钮检测相关的时钟和GPIO
@params	None
@retval	None
*/
void infrared_init(void){
	rcc_init();
	gpio_init();
	exti_init();
	nvic_init();

}

/*
@brief	初始化与按钮检测相关的时钟和GPIO
@params	None
@retval	0 按钮松开；1 按钮按下
*/


/*
@brief	初始化与按钮检测相关的时钟
@params	None
@retval	None
@note	这是一个私有函数
*/
static void rcc_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	//使用外部中断需要开启AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}

/*
@brief	初始化与按钮检测相关的GPIO
@params	None
@retval	None
@note	这是一个私有函数
*/
static void gpio_init(void){
	GPIO_InitTypeDef gpio_init;
	//设置PA0为上拉输入
	gpio_init.GPIO_Mode=GPIO_Mode_IPU;
	gpio_init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&gpio_init);
	gpio_init.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&gpio_init);
}

/*
@brief	初始化外部外部中断
@params	None
@retval	None
@note	这是一个私有函数
*/
static void exti_init(void)
{
	EXTI_InitTypeDef exti_init;
	

	
	exti_init.EXTI_Line=EXTI_Line12;              //外部中断线12   
	exti_init.EXTI_LineCmd=ENABLE;               //外部中断使能 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //外部中断模式 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //采用上升沿下降沿触发中断 
	EXTI_Init(&exti_init);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);    //配置连接引脚PC12
	

	
	exti_init.EXTI_Line=EXTI_Line14;              //外部中断线14
		exti_init.EXTI_LineCmd=ENABLE;               //外部中断使能 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //外部中断模式 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //采用上升沿下降沿触发中断 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);    //配置连接引脚PC14
	
	exti_init.EXTI_Line=EXTI_Line15;              //外部中断线15 
		exti_init.EXTI_LineCmd=ENABLE;               //外部中断使能 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //外部中断模式 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //采用上升沿下降沿触发中断 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);    //配置连接引脚PC15
	
		exti_init.EXTI_Line=EXTI_Line8;              //外部中断线15 
		exti_init.EXTI_LineCmd=ENABLE;               //外部中断使能 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //外部中断模式 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //采用上升沿下降沿触发中断 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);    //配置连接引脚PC15
	
}

/*
@brief	配置中断
@params	None
@retval	None
@note	这是一个私有函数
*/
static void nvic_init(void)
{
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel=EXTI15_10_IRQn;                //外部12中断 
	nvic_init.NVIC_IRQChannelCmd=ENABLE;                 //中断使能 
	nvic_init.NVIC_IRQChannelPreemptionPriority=0;       //抢占优先级0
	nvic_init.NVIC_IRQChannelSubPriority=0;              //响应优先级0 
	NVIC_Init(&nvic_init);
	
	nvic_init.NVIC_IRQChannel=EXTI9_5_IRQn;                //外部12中断 
	nvic_init.NVIC_IRQChannelCmd=ENABLE;                 //中断使能 
	nvic_init.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级0
	nvic_init.NVIC_IRQChannelSubPriority=1;              //响应优先级0 
	NVIC_Init(&nvic_init);
}