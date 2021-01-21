#include "infrared.h"


int volatile infrared_num=0;//�ڼ����ж�
int volatile infrared_stop=0;//�Ƿ�Ҫ��С��ֹͣ
/*
@brief	��ʼ���밴ť�����ص�ʱ�Ӻ�GPIO
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
@brief	��ʼ���밴ť�����ص�ʱ�Ӻ�GPIO
@params	None
@retval	0 ��ť�ɿ���1 ��ť����
*/


/*
@brief	��ʼ���밴ť�����ص�ʱ��
@params	None
@retval	None
@note	����һ��˽�к���
*/
static void rcc_init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	//ʹ���ⲿ�ж���Ҫ����AFIOʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
}

/*
@brief	��ʼ���밴ť�����ص�GPIO
@params	None
@retval	None
@note	����һ��˽�к���
*/
static void gpio_init(void){
	GPIO_InitTypeDef gpio_init;
	//����PA0Ϊ��������
	gpio_init.GPIO_Mode=GPIO_Mode_IPU;
	gpio_init.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOB,&gpio_init);
	gpio_init.GPIO_Pin=GPIO_Pin_8;
	GPIO_Init(GPIOA,&gpio_init);
}

/*
@brief	��ʼ���ⲿ�ⲿ�ж�
@params	None
@retval	None
@note	����һ��˽�к���
*/
static void exti_init(void)
{
	EXTI_InitTypeDef exti_init;
	

	
	exti_init.EXTI_Line=EXTI_Line12;              //�ⲿ�ж���12   
	exti_init.EXTI_LineCmd=ENABLE;               //�ⲿ�ж�ʹ�� 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //�ⲿ�ж�ģʽ 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //�����������½��ش����ж� 
	EXTI_Init(&exti_init);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);    //������������PC12
	

	
	exti_init.EXTI_Line=EXTI_Line14;              //�ⲿ�ж���14
		exti_init.EXTI_LineCmd=ENABLE;               //�ⲿ�ж�ʹ�� 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //�ⲿ�ж�ģʽ 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //�����������½��ش����ж� 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);    //������������PC14
	
	exti_init.EXTI_Line=EXTI_Line15;              //�ⲿ�ж���15 
		exti_init.EXTI_LineCmd=ENABLE;               //�ⲿ�ж�ʹ�� 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //�ⲿ�ж�ģʽ 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //�����������½��ش����ж� 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);    //������������PC15
	
		exti_init.EXTI_Line=EXTI_Line8;              //�ⲿ�ж���15 
		exti_init.EXTI_LineCmd=ENABLE;               //�ⲿ�ж�ʹ�� 
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;     //�ⲿ�ж�ģʽ 
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;   //�����������½��ش����ж� 
	EXTI_Init(&exti_init);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);    //������������PC15
	
}

/*
@brief	�����ж�
@params	None
@retval	None
@note	����һ��˽�к���
*/
static void nvic_init(void)
{
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel=EXTI15_10_IRQn;                //�ⲿ12�ж� 
	nvic_init.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ʹ�� 
	nvic_init.NVIC_IRQChannelPreemptionPriority=0;       //��ռ���ȼ�0
	nvic_init.NVIC_IRQChannelSubPriority=0;              //��Ӧ���ȼ�0 
	NVIC_Init(&nvic_init);
	
	nvic_init.NVIC_IRQChannel=EXTI9_5_IRQn;                //�ⲿ12�ж� 
	nvic_init.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ʹ�� 
	nvic_init.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�0
	nvic_init.NVIC_IRQChannelSubPriority=1;              //��Ӧ���ȼ�0 
	NVIC_Init(&nvic_init);
}