/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usart.h"
#include "infrared.h"
#include "motor.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*
void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
/*void SysTick_Handler(void)
{
}*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
//命令串口中断
void USART1_IRQHandler(void){	
	
	while(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET) {
		
		uint8_t data;
       data = USART_ReceiveData(USART1);
		if(data<'0')continue;
			usart1Buf[usart1Index++] = data;
			 USART_SendData(USART1,data);//发送读到的数据
	//GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}

	if(usart1Index==6){
	usart1Index=0;
	isNew=1;//有新的控制信号
	}
	
USART_ClearFlag(USART1,USART_IT_RXNE); //一定要清除接收中断 
}

//红外中断
void EXTI15_10_IRQHandler(void)
{
//USART_SendData(USART1,1);//发送读到的数据
if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==0||GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==0){	
if((usart1Buf[0]-48)==0||(usart1Buf[0]-48)==1){
		infrared_stop=1;//停止小车
	if((usart1Buf[0]-48)==0){
	motor_direction(1);
  motor_pwm(0,0);
  motor_pwm(1,0);
	motor_pwm(2,0);
	motor_pwm(3,0);

	}
	else{
		motor_direction(0);
  motor_pwm(0,0);
  motor_pwm(1,0);
	motor_pwm(2,0);
	motor_pwm(3,0);

	}
}
}

//	infrared_stop=0;
	EXTI_ClearITPendingBit(EXTI_Line12);	//清除中断标志
	
	EXTI_ClearITPendingBit(EXTI_Line14);
	EXTI_ClearITPendingBit(EXTI_Line15);
	
}

void EXTI9_5_IRQHandler(void)
{
//USART_SendData(USART1,1);//发送读到的数据
	
if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0){	
if((usart1Buf[0]-48)==0||(usart1Buf[0]-48)==1){
	infrared_stop=1;//停止小车
	if((usart1Buf[0]-48)==0){
	motor_direction(1);
  motor_pwm(0,0);
  motor_pwm(1,0);
	motor_pwm(2,0);
	motor_pwm(3,0);


	}
	else{
		motor_direction(0);
  motor_pwm(0,0);
  motor_pwm(1,0);
	motor_pwm(2,0);
	motor_pwm(3,0);

	}
}
}

//	infrared_stop=0;
	EXTI_ClearITPendingBit(EXTI_Line8);	//清除中断标志

	
}
