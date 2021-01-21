//�������������
#include "motor.h"

//����˵Ķ���
 unsigned int before_0=1;
 unsigned int before_1=0;
 unsigned int before_2=0;
 unsigned int before_3=1;
 
 unsigned int after_0=0;
 unsigned int after_1=1;
 unsigned int after_2=0;
 unsigned int after_3=1;

//ʹ�ܶ˵Ķ���
 unsigned int before_pwmA=0;
 unsigned int before_pwmB=0;
 unsigned int after_pwmA=0;
 unsigned int after_pwmB=0;
 
 

 
//ʹ�����������Ч
void motor_gpio_change(void){
	
	//ǰ���0
	if(before_0==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
			
	
	}

	//ǰ���1
		if(before_1==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	}
		//ǰ���2
		if(before_2==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
	
	}
		//ǰ���3
		if(before_3==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	
	}
	//����0
	if(after_0==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	
	}
	//����1
	if(after_1==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	
	}
		//����2
	if(after_2==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
	
	}
		//����3
	if(after_3==0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	
	}
	else{
	
	GPIO_SetBits(GPIOB,GPIO_Pin_11);
	
	}




}

//ʱ������
 void motor_rcc_config(void){
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//���ù���ʱ��
 }

 //gpio��������
void motor_gpio_config(void){
GPIO_InitTypeDef gpio_init;

	//1.1���ʹ�ܶ˵Ķ���
	gpio_init.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	
	//����A ����B
	gpio_init.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpio_init);

	
	//ǰ���A ǰ���B
	gpio_init.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&gpio_init);
	

	
	//1.2����˿ڵĶ���
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	
	//����0  ����1  ����2 ����3
	gpio_init.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB,&gpio_init);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);//�ͷ�B4����

	// ǰ���0	 ǰ���1	 	ǰ���2		ǰ���3		
	gpio_init.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init(GPIOB,&gpio_init);

	

	//2.1ʹ�ܶ˿ڵĳ�ʼֵ
    motor_gpio_change();
	
	
	
	
}

//��������Ч
 void motor_tim_change(void){
 TIM4->CCR3=before_pwmA;
 TIM4->CCR4=before_pwmB;
 TIM3->CCR1=after_pwmA;
 TIM3->CCR2=after_pwmB;
 
 
 
 }

 //��ʱ������
void motor_tim_config(void){
	TIM_TimeBaseInitTypeDef tim_base_init;
	TIM_OCInitTypeDef tim_oc_init;
	
	//1��TIM������
	
	//TIM3����
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ
	tim_base_init.TIM_Period=TIM3_period;						//����װ�ؼ�������ֵ
	tim_base_init.TIM_Prescaler=0;						//���÷�Ƶ 
	tim_base_init.TIM_ClockDivision =0;  
	TIM_TimeBaseInit(TIM3,&tim_base_init);
	
	//TIM4����
	TIM_TimeBaseStructInit(&tim_base_init);
	tim_base_init.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ
	tim_base_init.TIM_Prescaler=0;						//���÷�Ƶ 
	tim_base_init.TIM_ClockDivision =0;  
	tim_base_init.TIM_Period=TIM1_period;						//����װ�ؼ�������ֵ
	TIM_TimeBaseInit(TIM4,&tim_base_init);
	
	//2��TIM���ͨ������
	
	//�������ͨ��3��4ΪPWM1ģʽ
	TIM_OCStructInit(&tim_oc_init);

	tim_oc_init.TIM_OCMode=TIM_OCMode_PWM1;					//���PWM1ģʽ
	tim_oc_init.TIM_OCPolarity=TIM_OCPolarity_High;				
	tim_oc_init.TIM_OutputState=TIM_OutputState_Enable;		//���״̬ʹ��
	tim_oc_init.TIM_Pulse=0;								//��ʼֵ����

   //����TIM3��1��2ͨ��
		TIM_OC1Init(TIM3,&tim_oc_init);
		TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
		TIM_OC2Init(TIM3,&tim_oc_init);
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
	   //ʹ��TIM3	
	   TIM_ARRPreloadConfig(TIM3,ENABLE);
	   TIM_Cmd(TIM3,ENABLE);

	
	 //����TIM4��1��2ͨ��
	  TIM_OC3Init(TIM4,&tim_oc_init);
		TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_OC4Init(TIM4,&tim_oc_init);
		TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
		

     //ʹ��TIM4
		 TIM_ARRPreloadConfig(TIM4,ENABLE);
	   TIM_Cmd(TIM4,ENABLE);
		 
		 //����������
		 motor_tim_change();

}

//�������������
 void motor_config(void){
 motor_rcc_config();
 motor_gpio_config();
 motor_tim_config();
 }
 
 
  //���ĵ������������
 //����ǰ�����Ǻ���,0����ǰ����1�������
 void motor_direction(int n){
	
 if(n==1){
 //����˵Ķ���
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
  //����˵Ķ���
 before_0=1;
 before_1=0;
 before_2=0;
 before_3=1;
 
 after_0=0;
 after_1=1;
 after_2=0;
 after_3=1;
 
 
 }
 
 motor_gpio_change();//ʹת����Ч
  
 }
 
 //��ʹ�ܶ˵�pwm,0����ǰA,1����ǰB��2�����A,3�����B
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
 
 