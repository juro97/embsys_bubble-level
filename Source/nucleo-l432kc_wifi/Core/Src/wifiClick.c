#include "wifiClick.h"


const char *xUartHandlerTaskName = "UartHandler";
const char *newLine = "\r\n";

uint8_t uart1Buffer[BUFFER_SIZE];
uint8_t uart2Buffer[BUFFER_SIZE];

uint16_t uart1BufferIndex = 0;
uint16_t uart2BufferIndex = 0;

uint8_t uart2_rx_char;

TaskHandle_t xUartTaskHandle = NULL;

char at_cmd_buffer[AT_CMD_BUFFER_SIZE] = {0};

void StartDefaultTask(void *argument) {

	xTaskCreate(UartHandlerTask, xUartHandlerTaskName, 128, NULL, osPriorityNormal1, &xUartTaskHandle);

	LL_USART_EnableIT_IDLE(USART1); 
	HAL_UART_Receive_DMA(&huart1, uart1Buffer, BUFFER_SIZE);

	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Set_Mode, "%u", AT_WIFI_Mixed_Mode);
	osDelay(10);
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Set_MultiConnectionMode, "%u", AT_IP_ConnectionMode_Multiple);
	osDelay(10);
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Server, "%u,%u", AT_IP_Server_Create, 80);
	osDelay(10);

	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Connect, "\"%s\",\"%s\"", "wasserwaage", "embsy2023");
	osDelay(10);
	at_execute_command(at_cmd_buffer, SendATCommand, AT_Startup);
	osDelay(10);

	HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);

	osThreadExit();
}

void UartHandlerTask(void *argument) {
	uint32_t ulNotificationValue;
	static size_t old_pos = 0;  

	while(1)
	{
		if(xTaskNotifyWait(0x00, UINT32_MAX, &ulNotificationValue, portMAX_DELAY) == pdPASS) {

			if(ulNotificationValue & UART1_IDLE_EVENT) 
			{
				size_t new_pos = BUFFER_SIZE - huart1.hdmarx->Instance->CNDTR;  
				size_t length;

				if(new_pos != old_pos)  
				{
					if (new_pos > old_pos)  
					{
						length = new_pos - old_pos;
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);
					}
					else 
					{
						length = BUFFER_SIZE - old_pos;
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);
						length = new_pos;
						HAL_UART_Transmit(&huart2, uart1Buffer, length, HAL_MAX_DELAY);
					}

					old_pos = new_pos;  
				}
			}
			if(ulNotificationValue & UART2_EVENT) 
			{
				char c = (char)uart2_rx_char;

				HAL_UART_Transmit_IT(&huart2, (uint8_t*)&c, 1);

				if(c == '\r')
				{
					uart2Buffer[uart2BufferIndex++] = '\r';
					uart2Buffer[uart2BufferIndex++] = '\n';
					HAL_UART_Transmit_IT(&huart2, (uint8_t*)newLine, 2);
				
					HAL_UART_Transmit(&huart1, uart2Buffer, uart2BufferIndex, HAL_MAX_DELAY);
					memset(uart2Buffer, 0, BUFFER_SIZE);
					uart2BufferIndex = 0;
				}
				else
				{
					uart2Buffer[uart2BufferIndex++] = c;
				}
			}
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2) {
	  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  xTaskNotifyFromISR(xUartTaskHandle, UART2_EVENT, eSetBits, &xHigherPriorityTaskWoken);
	  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	  HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
  }
}

void SendATCommand(char *command, int length) {
	HAL_UART_Transmit(&huart1, (uint8_t*)command, length, HAL_MAX_DELAY);
}
