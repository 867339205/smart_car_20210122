#include "sys.h"
#include "delay.h"	
#include "includes.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "motor.h"
#include "usart.h"
#include "rudder.h"
#include "infrared.h"

/////////////////////////互斥信号量///////////////////////////////////
#define MUTEX_PRIO    6 //互斥信号量优先级
OS_EVENT *pmutex;       //互斥信号量指针

#define MUTEX_PRIO_1    5 //互斥信号量优先级
OS_EVENT *pmutex_1;       //互斥信号量指针

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
//设置任务优先级
#define START_TASK_PRIO					10
//设置任务堆栈大小
#define START_STK_SIZE					64
//任务堆栈	
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);




//舵机任务，红外
//设置任务优先级
#define DG_TASK_PRIO					7
//设置任务堆栈大小
#define DG_STK_SIZE					512
//任务堆栈	
OS_STK DG_TASK_STK[DG_STK_SIZE];
//任务函数
void DG_task(void *pdata);

//LED1任务
//设置任务优先级
#define LED1_TASK_PRIO					8
//设置任务堆栈大小
#define LED1_STK_SIZE					64
//任务堆栈	
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void LED1_task(void *pdata);

//串口控制命令任务
//设置任务优先级
#define USART1_TASK_PRIO					9
//设置任务堆栈大小
#define USART1_STK_SIZE					1024
//任务堆栈	
OS_STK USART1_TASK_STK[USART1_STK_SIZE];
//任务函数
void usart1_task(void *pdata);


/////////////////////////全局变量///////////////////////////////////
int check=0;
OS_TMR   * tmr1;            //软件定时器1
	INT8U err;//用来接收错误信息



int main(void){
   uint8_t data;
   //Stm32_Clock_Init(9);	//系统时钟设置
	 delay_init(72);			//延时初始化

   motor_config();//电机驱动初始化
	 usart_init();	//初始化串口
   rudder_init();//初始化舵机
	 infrared_init();//初始化红外


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//进行中断分组，分为2组：2位抢占优先级，2位响应优先级
	
	OSInit();				//初始化uCOSii
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();				//启动系统
    


	
	}

	//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata;
	pmutex=OSMutexCreate(MUTEX_PRIO,&err);//舵机互斥
	pmutex_1=OSMutexCreate(MUTEX_PRIO_1,&err);//灯互斥
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)    
	OSTaskCreate(DG_task,(void *)0,(OS_STK*)&DG_TASK_STK[DG_STK_SIZE-1],DG_TASK_PRIO);
	OSTaskCreate(LED1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
	OSTaskCreate(usart1_task,(void *)0,(OS_STK*)&USART1_TASK_STK[USART1_STK_SIZE-1],USART1_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}




//定时器回调函数
void tmr1_callback(OS_TMR *ptmr,void *p_arg)
{
    check=0;
	
	OSTmrStop(tmr1,OS_TMR_OPT_NONE,0,&err);	//关闭软件定时器1
	DG_check=0;
	//OSMutexPend(pmutex,0,&err);//等待信号量
	//delay_ms(600);
}
//舵机，红外，超神波任务
void DG_task(void *pdata)
{	 	
	DG_check=1;
	while(1) {
		OSMutexPend(pmutex,0,&err);//等待信号量
 if(DG_check==1&&check==0){

    tmr1=OSTmrCreate(0,20,OS_TMR_OPT_ONE_SHOT ,(OS_TMR_CALLBACK)tmr1_callback,0,(INT8U*)"tmr1",&err);//500ms执行一次
    OSTmrStart(tmr1,&err);//启动软件定时器1
	 check=1;
	OSMutexPost(pmutex);//释放信号量
		}
 else if(DG_check==1&&check==1){
 
 rudder_change();
	 OSMutexPost(pmutex);//释放信号量
 }
	  else{
	  OSMutexPost(pmutex);//释放信号量
		delay_ms(5);
		}

	}
}

//LED1任务
void LED1_task(void *pdata)
{	 	
	//usart_send_byte('0');
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	

	gpio_init.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOC,&gpio_init);
	while(1) {
		
	OSMutexPend(pmutex_1,0,&err);//等待信号量
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		//delay_ms(600);
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//	delay_ms(1000);
		OSMutexPost(pmutex_1);//释放信号量
		delay_ms(600);
	}
}

//串口控制命令任务,包括其他处理
void usart1_task(void *pdata){
	isNew=0;//是否有新的串口数据
	
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&gpio_init);
	
	OSMutexPend(pmutex,0,&err);//等待信号量
while(1){		
//OSMutexPend(pmutex,0,&err);//等待信号量

	//接收到串口数据
if(isNew==1){
	OSMutexPend(pmutex_1,0,&err);//等待信号量	
		OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断) 
	if((usart1Buf[0]-48)==0||(usart1Buf[0]-48)==1){
  
		//设置灯闪烁
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
		//发送接收到的数据
		usart_send_byte(usart1Buf[0]-48+16);
		usart_send_byte(usart1Buf[1]-48+16);
		usart_send_byte(usart1Buf[2]-48+16);
		usart_send_byte(usart1Buf[3]-48+16);
		usart_send_byte(usart1Buf[4]-48+16);	
		
		motor_direction(usart1Buf[0]-48);//设置车子的前进后退		
		if((usart1Buf[1]-48)==0){
		motor_pwm(0,0);
		}
		else{
				motor_pwm(0,(usart1Buf[1]-48)*60+460);
		}

		if((usart1Buf[2]-48)==0){
		motor_pwm(1,0);
		}
		else{
		motor_pwm(1,(usart1Buf[2]-48)*60+460);
		}		

		if((usart1Buf[3]-48)==0){
		motor_pwm(2,0);
		}
		else{
		motor_pwm(2,(usart1Buf[3]-48)*60+460);
		}

		if((usart1Buf[4]-48)==0){
		motor_pwm(3,0);
		}
		else{
		motor_pwm(3,(usart1Buf[4]-48)*60+460);
		}





		//发送设置后的数据		
		usart_send_byte((TIM4->CCR3-460)/60);
		usart_send_byte((TIM4->CCR4-460)/60);
		usart_send_byte((TIM3->CCR1-460)/60);	
		usart_send_byte((TIM3->CCR2-460)/60);
		
		//设置喇叭
		if((usart1Buf[5]-48)==1){
			
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		
		}
		else{
		
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		
		}
	}
	
		isNew=0;
		OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
	OSMutexPost(pmutex_1);
}
//遇到障碍物
if(infrared_stop){
DG_check=1;
	DG_n=2550;
OSMutexPost(pmutex);//释放信号量
	while(DG_check){
	
	}
	
DG_check=1;
	DG_n=700;
OSMutexPost(pmutex);//释放信号量
	while(DG_check){
	
	}
	
DG_check=1;
	DG_n=1650;
OSMutexPost(pmutex);//释放信号量
	while(DG_check){
	
	}
	

OSMutexPend(pmutex,0,&err);//等待信号量
infrared_stop=0;
}

}

}