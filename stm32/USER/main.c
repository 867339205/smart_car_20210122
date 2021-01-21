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

/////////////////////////�����ź���///////////////////////////////////
#define MUTEX_PRIO    6 //�����ź������ȼ�
OS_EVENT *pmutex;       //�����ź���ָ��

#define MUTEX_PRIO_1    5 //�����ź������ȼ�
OS_EVENT *pmutex_1;       //�����ź���ָ��

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
//�����������ȼ�
#define START_TASK_PRIO					10
//���������ջ��С
#define START_STK_SIZE					64
//�����ջ	
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);




//������񣬺���
//�����������ȼ�
#define DG_TASK_PRIO					7
//���������ջ��С
#define DG_STK_SIZE					512
//�����ջ	
OS_STK DG_TASK_STK[DG_STK_SIZE];
//������
void DG_task(void *pdata);

//LED1����
//�����������ȼ�
#define LED1_TASK_PRIO					8
//���������ջ��С
#define LED1_STK_SIZE					64
//�����ջ	
OS_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void LED1_task(void *pdata);

//���ڿ�����������
//�����������ȼ�
#define USART1_TASK_PRIO					9
//���������ջ��С
#define USART1_STK_SIZE					1024
//�����ջ	
OS_STK USART1_TASK_STK[USART1_STK_SIZE];
//������
void usart1_task(void *pdata);


/////////////////////////ȫ�ֱ���///////////////////////////////////
int check=0;
OS_TMR   * tmr1;            //�����ʱ��1
	INT8U err;//�������մ�����Ϣ



int main(void){
   uint8_t data;
   //Stm32_Clock_Init(9);	//ϵͳʱ������
	 delay_init(72);			//��ʱ��ʼ��

   motor_config();//���������ʼ��
	 usart_init();	//��ʼ������
   rudder_init();//��ʼ�����
	 infrared_init();//��ʼ������


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϷ��飬��Ϊ2�飺2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	OSInit();				//��ʼ��uCOSii
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();				//����ϵͳ
    


	
	}

	//��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata = pdata;
	pmutex=OSMutexCreate(MUTEX_PRIO,&err);//�������
	pmutex_1=OSMutexCreate(MUTEX_PRIO_1,&err);//�ƻ���
	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��)    
	OSTaskCreate(DG_task,(void *)0,(OS_STK*)&DG_TASK_STK[DG_STK_SIZE-1],DG_TASK_PRIO);
	OSTaskCreate(LED1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);
	OSTaskCreate(usart1_task,(void *)0,(OS_STK*)&USART1_TASK_STK[USART1_STK_SIZE-1],USART1_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����
	OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
}




//��ʱ���ص�����
void tmr1_callback(OS_TMR *ptmr,void *p_arg)
{
    check=0;
	
	OSTmrStop(tmr1,OS_TMR_OPT_NONE,0,&err);	//�ر������ʱ��1
	DG_check=0;
	//OSMutexPend(pmutex,0,&err);//�ȴ��ź���
	//delay_ms(600);
}
//��������⣬��������
void DG_task(void *pdata)
{	 	
	DG_check=1;
	while(1) {
		OSMutexPend(pmutex,0,&err);//�ȴ��ź���
 if(DG_check==1&&check==0){

    tmr1=OSTmrCreate(0,20,OS_TMR_OPT_ONE_SHOT ,(OS_TMR_CALLBACK)tmr1_callback,0,(INT8U*)"tmr1",&err);//500msִ��һ��
    OSTmrStart(tmr1,&err);//���������ʱ��1
	 check=1;
	OSMutexPost(pmutex);//�ͷ��ź���
		}
 else if(DG_check==1&&check==1){
 
 rudder_change();
	 OSMutexPost(pmutex);//�ͷ��ź���
 }
	  else{
	  OSMutexPost(pmutex);//�ͷ��ź���
		delay_ms(5);
		}

	}
}

//LED1����
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
		
	OSMutexPend(pmutex_1,0,&err);//�ȴ��ź���
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		//delay_ms(600);
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//	delay_ms(1000);
		OSMutexPost(pmutex_1);//�ͷ��ź���
		delay_ms(600);
	}
}

//���ڿ�����������,������������
void usart1_task(void *pdata){
	isNew=0;//�Ƿ����µĴ�������
	
	GPIO_InitTypeDef gpio_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio_init.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed=GPIO_Speed_50MHz;
	gpio_init.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&gpio_init);
	
	OSMutexPend(pmutex,0,&err);//�ȴ��ź���
while(1){		
//OSMutexPend(pmutex,0,&err);//�ȴ��ź���

	//���յ���������
if(isNew==1){
	OSMutexPend(pmutex_1,0,&err);//�ȴ��ź���	
		OS_CPU_SR cpu_sr=0;
	OS_ENTER_CRITICAL();			//�����ٽ���(�޷����жϴ��) 
	if((usart1Buf[0]-48)==0||(usart1Buf[0]-48)==1){
  
		//���õ���˸
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
		//���ͽ��յ�������
		usart_send_byte(usart1Buf[0]-48+16);
		usart_send_byte(usart1Buf[1]-48+16);
		usart_send_byte(usart1Buf[2]-48+16);
		usart_send_byte(usart1Buf[3]-48+16);
		usart_send_byte(usart1Buf[4]-48+16);	
		
		motor_direction(usart1Buf[0]-48);//���ó��ӵ�ǰ������		
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





		//�������ú������		
		usart_send_byte((TIM4->CCR3-460)/60);
		usart_send_byte((TIM4->CCR4-460)/60);
		usart_send_byte((TIM3->CCR1-460)/60);	
		usart_send_byte((TIM3->CCR2-460)/60);
		
		//��������
		if((usart1Buf[5]-48)==1){
			
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		
		}
		else{
		
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		
		}
	}
	
		isNew=0;
		OS_EXIT_CRITICAL();				//�˳��ٽ���(���Ա��жϴ��)
	OSMutexPost(pmutex_1);
}
//�����ϰ���
if(infrared_stop){
DG_check=1;
	DG_n=2550;
OSMutexPost(pmutex);//�ͷ��ź���
	while(DG_check){
	
	}
	
DG_check=1;
	DG_n=700;
OSMutexPost(pmutex);//�ͷ��ź���
	while(DG_check){
	
	}
	
DG_check=1;
	DG_n=1650;
OSMutexPost(pmutex);//�ͷ��ź���
	while(DG_check){
	
	}
	

OSMutexPend(pmutex,0,&err);//�ȴ��ź���
infrared_stop=0;
}

}

}