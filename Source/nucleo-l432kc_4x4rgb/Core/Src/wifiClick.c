#include <wifiClick.h>

const char *xUartHandlerTaskName = "UartHandler";
const char *newLine = "\r\n";

// Create circular buffers
uint8_t uart1Buffer[BUFFER_SIZE];
uint8_t uart2Buffer[BUFFER_SIZE];

// Buffer indices
uint16_t uart1BufferIndex = 0;
uint16_t uart2BufferIndex = 0;

// temporaray data buffers
uint8_t uart2_rx_char;

// Task handles
TaskHandle_t xUartTaskHandle = NULL;

char at_cmd_buffer[AT_CMD_BUFFER_SIZE] = {0};

void StartWifiClick(void *argument) {
	// Create tasks
	//xTaskCreate(uart2Task, "Uart1Task", 128, NULL, osPriorityLow, NULL);
	xTaskCreate(UartHandlerTask, xUartHandlerTaskName, 256, &argument, osPriorityNormal1, &xUartTaskHandle);

	// Activate UART interrupts and reception
	LL_USART_EnableIT_IDLE(USART1); // Enable idle line detection (interrupt) for uart1
	HAL_UART_Receive_DMA(&huart1, uart1Buffer, BUFFER_SIZE);

	// Configure Station Mode
	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Set_Mode, "%u", AT_WIFI_Station_Mode);
	osDelay(10);
	// Allow multiple connections
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Set_MultiConnectionMode, "%u", AT_IP_ConnectionMode_Multiple);
	osDelay(10);
	// Start TCP server on Port 80
	at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Server, "%u,%u", AT_IP_Server_Create, 80);
	osDelay(10);
	// CONNECT to wifi
	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Connect, "\"%s\",\"%s\"", "ssid", "pw");
	osDelay(10);
	at_execute_command(at_cmd_buffer, SendATCommand, AT_Startup);
	osDelay(10);

	HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
}

void UartHandlerTask(char * ch) {
	uint32_t ulNotificationValue;
	static size_t old_pos = 0;  // Track the position of last character processed
	size_t startPos = 0;
	size_t currIndex = 0;
    char prefix[] = "+IPD,0,3:";
	while(1)
	{
		// Wait for a task notification indicating an uart event
		if(xTaskNotifyWait(0x00, UINT32_MAX, &ulNotificationValue, portMAX_DELAY) == pdPASS) {

			if(ulNotificationValue & UART1_IDLE_EVENT) // Received message from UART1
			{
				size_t new_pos = BUFFER_SIZE - huart1.hdmarx->Instance->CNDTR;  // Compute the new position in the buffer
				size_t length;

				if(new_pos != old_pos)  // Check if any new data is received
				{
					if (new_pos > old_pos)  // If data does not wrap around the buffer
					{
						length = new_pos - old_pos;
						
						/*
						 * Process here
						 * */
						for(int i = 0;i<length;i++){
							currIndex = old_pos + i;
							if(uart1Buffer[currIndex] == '\n' || uart1Buffer[currIndex] == '\r')continue;

							if(strlen(prefix)==i-startPos)
							{
								printDataOnMatrix(&uart1Buffer[currIndex]);
								break;
							}


							if(uart1Buffer[currIndex] == prefix[0])startPos = i;
							else if(uart1Buffer[currIndex] != prefix[i-startPos])break;

						}

						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);
					}
					else  // If data wraps around the buffer
					{
						/*
						 * Process here --> keep in mind, that the data has to be composed, as it wraps around the buffer
						 * */

						length = BUFFER_SIZE - old_pos;
						HAL_UART_Transmit(&huart2, &uart1Buffer[old_pos], length, HAL_MAX_DELAY);

						length = new_pos;
						HAL_UART_Transmit(&huart2, uart1Buffer, length, HAL_MAX_DELAY);
					}

					old_pos = new_pos;  // Update the position of the last character processed
				}
			}
			if(ulNotificationValue & UART2_EVENT) // Received CHARACTER from UART2
			{
				char c = (char)uart2_rx_char;

				// Echo back the character to the terminal
				HAL_UART_Transmit_IT(&huart2, (uint8_t*)&c, 1);

				// If newline, forward the buffer to UART1 and reset buffer
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

// Interrupt callback routine for UART
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
	taskENTER_CRITICAL();
	HAL_UART_Transmit(&huart1, (uint8_t*)command, length, HAL_MAX_DELAY);
	taskEXIT_CRITICAL();
}