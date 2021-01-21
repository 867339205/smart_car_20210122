//电机的驱动程序
#include "motor.h"

//输入端的定义
 unsigned int before_0=1;
 unsigned int before_1=0;
 unsigned int before_2=0;
 unsigned int before_3=1;
 
 unsigned int after_0=0;
 unsigned int after_1=1;
 unsigned int after_2=0;
 unsigned int after_3=1;

//使能端的定义
 unsigned int before_pwmA=0;
 unsigned int before_pwmB=0;
 unsigned int after_pwmA=0;
 unsigned int after_pwmB=0;
 
 

 
//使输入端数据生效
void motor_gpio_change(void){
	
	//前电机0
	if(before_0==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
			
	
	}

	//前电机1
		if(before_1==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	}
		//前电机2
		if(before_2==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
	}
		//前电机3
		if(before_3==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
	}
	//后电机0
	if(after_0==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	}
	//后电机1
	if(after_1==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	
	}
		//后电机2
	if(after_2==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	
	}
		//后电机3
	if(after_3==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	
	}




}

//时钟配置
 void motor_rcc_config(void){
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//复用功能时钟
 }

 //gpio引脚配置
void motor_gpio_config(void){
GPIO_InitTypeDef gpio_init;

	//1.1电机使能端的定义
	gpio_init.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	
	//后电机A 后电机B
	gpio_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpio_init);

	
	//前电机A 前电机B
	gpio_init.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&gpio_init);
	

	
	//1.2输入端口的定义
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	
	//后电机0  后电机1  后电机2 后电机3
	gpio_init.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB,&gpio_init);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);//释放B4引脚

	// 前电机0	 前电机1	 	前电机2		前电机3		
	gpio_init.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB,&gpio_init);

	

	//2.1使能端口的初始值
    motor_gpio_change();
	
	
	
	
}

//脉冲宽度生效
 void motor_tim_change(void){
 TIM4->CCR3=before_pwmA;
 TIM4->CCR4=before_pwmB;
 TIM3->CCR1=after_pwmA;
 TIM3->CCR2=after_pwmB;
 
 
 
 }

 //定时器配置
void motor_tim_config(void){
	TIM_TimeBaseInitTypeDef tim_base_init;
	TIM_OCInitTypeDef tim_oc_init;
	
	//1、TIM的设置
	
	//TIM3设置
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式
	tim_base_init.TIM_Period=TIM3_period;						//设置装载计数周期值
	tim_base_init.TIM_Prescaler=0;						//设置分频 
	tim_base_init.TIM_ClockDivision =0;  
	TIM_TimeBaseInit(TIM3,&tim_base_init);
	
	//TIM4设置
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式
	tim_base_init.TIM_Prescaler=0;						//设置分频 
	tim_base_init.TIM_ClockDivision =0;  
	tim_base_init.TIM_Period=TIM1_period;						//设置装载计数周期值
	TIM_TimeBaseInit(TIM4,&tim_base_init);
	
	//2、TIM输出通道配置
	
	//设置输出通道3、4为PWM1模式
	TIM_OCStructInit(&tim_oc_init);

	tim_oc_init.TIM_OCMode=TIM_OCMode_PWM1;					//输出PWM1模式
	tim_oc_init.TIM_OCPolarity=TIM_OCPolarity_High;				
	tim_oc_init.TIM_OutputState=TIM_OutputState_Enable;		//输出状态使能
	tim_oc_init.TIM_Pulse=0;								//初始值而已

   //配置TIM3的1、2通道
		TIM_OC1Init(TIM3,&tim_oc_init);
		TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
		TIM_OC2Init(TIM3,&tim_oc_init);
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
	   //使能TIM3	
	   TIM_ARRPreloadConfig(TIM3,ENABLE);
	   TIM_Cmd(TIM3,ENABLE);

	
	 //配置TIM4的1、2通道
	  TIM_OC3Init(TIM4,&tim_oc_init);
		TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_OC4Init(TIM4,&tim_oc_init);
		TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
		

     //使能TIM4
		 TIM_ARRPreloadConfig(TIM4,ENABLE);
	   TIM_Cmd(TIM4,ENABLE);
		 
		 //配置脉冲宽度
		 motor_tim_change();

}

//电机驱动的配置
 void motor_config(void){
 motor_rcc_config();
 motor_gpio_config();
 motor_tim_config();
 }
 
 
  //更改电机驱动的配置
 //车子前进还是后退,0代表前进，1代表后退
 void motor_direction(int n){
	
 if(n==1){
 //输入端的定义
 before_0=0;
 before_1=1;
 before_2=1;
 before_3=0;
 
 after_0=1;
 after_1=0;
 after_2=1;
 after_3=0;
 
 
 }
 else{
  //输入端的定义
 before_0=1;
 before_1=0;
 before_2=0;
 before_3=1;
 
 after_0=0;
 after_1=1;
 after_2=0;
 after_3=1;
 
 
 }
 
 motor_gpio_change();//使转向生效
  
 }
 
 //改使能端的pwm,0代表前A,1代表前B，2代表后A,3代表后B
 void motor_pwm(int n,unsigned int pwm){
 if(n==0){
 before_pwmA=pwm;
 }
 else if(n==1){
 
	 if(pwm>BLance){
	 before_pwmB=pwm-BLance;
	 }
	 else{
	 before_pwmB=pwm;
	 
	 }
 
 }
 else if(n==2){

 	 if(pwm>BLance){
	 after_pwmA=pwm-BLance;
	 }
	 else{
	 after_pwmA=pwm;
	 
	 }
 
 }
 else{
 
   after_pwmB=pwm;
 
 
 }
 motor_tim_change();
 }
 
 