#include "my_usart.h"
static const char *TAG = "uart_events";
const int uart_num = UART_NUM_2;//使用串口2
static QueueHandle_t uart2_queue;


static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t* dtmp = (uint8_t*) malloc(BUF_SIZE);
    int len;//数据长度
    int i=0;//用来循环

    char data[20];//串口接收到的数据


    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart2_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.
                in this example, we don't process data in event, but read data outside.*/
                case UART_DATA:
                    uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                    ESP_LOGI(TAG, "data, len: %d; buffered len: %d", event.size, buffered_size);
                    printf("收到串口数据\n");
                    fflush(stdout);
                    len=usart_get(data);
                    for(i=0;i<len;i++){
                    	printf("%08x\n",data[i]);


                    }

                    fflush(stdout);
                    break;
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow\n");
                    //If fifo overflow happened, you should consider adding flow control for your application.
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full\n");
                    //If buffer full happened, you should consider encreasing your buffer size
                    //We can read data out out the buffer, or directly flush the rx buffer.
                    uart_flush(EX_UART_NUM);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break\n");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error\n");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error\n");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    ESP_LOGI(TAG, "uart pattern detected\n");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d\n", event.type);
                    break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}


//初始化串口0
 void usart_init()
{

    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
		.rx_flow_ctrl_thresh = 122,

    };

    uart_param_config(uart_num, &uart_config);

    uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(uart_num, BUF_SIZE * 2, BUF_SIZE * 2, 10, &uart2_queue, 0);

    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);

}

//发送数据到串口，参数是要发送的字符数组
void usart_send(char* data)
{
	int len=strlen(data);
	uart_write_bytes(uart_num, (const char*) data, len);
	printf("发送串口数据成功,%c\n",data[0]);
	 fflush(stdout);

}

//接收串口数据，参数是用来接收数据的指针
int usart_get(char* data){

	int len = uart_read_bytes(uart_num, (uint8_t*)data, BUF_SIZE, 20 / portTICK_RATE_MS);

	return len;
}




