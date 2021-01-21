#include "my_usart.h"
#include "my_blu.h"

void app_main()
{
    //xTaskCreate(&blu_init, "blu_init", 2048, NULL, 5, NULL);//创建蓝牙进程
    //xTaskCreate(&usart_init, "usart_init", 2048, NULL, 5, NULL);//创建串口进程
	usart_init();//初始化串口
	//printf("初始化蓝牙\n");
	// fflush(stdout);
	blu_init();//初始化蓝牙

}


