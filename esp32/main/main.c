#include "my_usart.h"
#include "my_blu.h"

void app_main()
{
    //xTaskCreate(&blu_init, "blu_init", 2048, NULL, 5, NULL);//������������
    //xTaskCreate(&usart_init, "usart_init", 2048, NULL, 5, NULL);//�������ڽ���
	usart_init();//��ʼ������
	//printf("��ʼ������\n");
	// fflush(stdout);
	blu_init();//��ʼ������

}


