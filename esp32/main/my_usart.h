#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"


#define ECHO_TEST_TXD  (17)
#define ECHO_TEST_RXD  (16)
//#define ECHO_TEST_RTS  (18)
//#define ECHO_TEST_CTS  (19)

#define BUF_SIZE (1024)    //����
#define EX_UART_NUM UART_NUM_2

 void usart_init();//���ڳ�ʼ��
 void usart_send(char* data);//���ʹ�������
 int usart_get(char*data);//���մ�������
