#include "rudder.h"

int volatile DG_n=1650; //定义舵机转动的位置，左2600，中1650，右700
int volatile DG_check=0;//检测是否需要转动舵机

void rudder_init(void){
	GPIO_InitTypeDef gpio_init;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//打开时钟
	
  //初始化pc14
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&gpio_init);

}


void rudder_change(void){

  GPIO_ResetBits(GPIOA,GPIO_Pin_0);
		delay_us(20000-DG_n);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
		delay_us(DG_n);

}
