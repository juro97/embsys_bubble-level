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

/**

    @brief Starts the WiFi click functionality.

    This function creates tasks, activates UART interrupts and reception, configures the station mode, allows multiple connections, 
	starts a TCP server on port 80, connects to WiFi, and initiates startup.
    @param argument Pointer to the argument passed to the function (unused).
    */


void StartWifiClick(void *argument) {
	xTaskCreate(UartHandlerTask, xUartHandlerTaskName, 256, &argument, osPriorityNormal1, &xUartTaskHandle);
	LL_USART_EnableIT_IDLE(USART1); // Enable idle line detection (interrupt) for uart1
	HAL_UART_Receive_DMA(&huart1, uart1Buffer, BUFFER_SIZE);

	at_set_command(at_cmd_buffer, SendATCommand, AT_WIFI_Set_Mode, "%u", AT_WIFI_Station_Mode);
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
}

/**

    @brief Task function for handling UART events and processing data.

    This task waits for task notifications indicating UART events and performs specific actions based on the received notifications. 
	It handles both UART1 and UART2 events, such as receiving messages, processing data, and forwarding data to the appropriate UART.

    @param[in] ch Pointer to the character array used for UART2 communication.
    */

void UartHandlerTask(char * ch) {
	uint32_t ulNotificationValue;
	static size_t old_pos = 0; 
	size_t startPos = 0;
	size_t currIndex = 0;
    char prefix[] = "+IPD,0,3:";
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
					else  
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

/**
    @brief UART receive complete callback.
    This function is called when UART data reception is complete.
    If the UART instance is USART2, it notifies the UART handler task about the event and resumes the task.
    It also restarts UART reception for USART2.
    @param huart Pointer to the UART handle structure.
    */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART2) {
	  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	  xTaskNotifyFromISR(xUartTaskHandle, UART2_EVENT, eSetBits, &xHigherPriorityTaskWoken);
	  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	  HAL_UART_Receive_IT(&huart2, &uart2_rx_char, 1);
  }
}

/**

    @brief Sends an AT command over UART.
    This function transmits the specified AT command over UART1 with the given length.
    It uses a critical section to ensure exclusive access to the UART transmission.
    @param command Pointer to the AT command string.
    @param length Length of the command string.
    */

void SendATCommand(char *command, int length) {
	taskENTER_CRITICAL();
	HAL_UART_Transmit(&huart1, (uint8_t*)command, length, HAL_MAX_DELAY);
	taskEXIT_CRITICAL();
}
