#ifndef INC_WIFICLICK_H_
#define INC_WIFICLICK_H_

#include "cmsis_os2.h"			// For CMSIS API functions
#include "FreeRTOS.h"			// For FreeRTOS API functions
#include "usart.h"				// For UART handles
#include "task.h"				// for FreeRTOS task control functions
#include "string.h"				// for string functions
#include "stdbool.h"			// for boolean values
#include "stm32l4xx_ll_usart.h" // For lowlevel usart abstraction access
#include "printf.h"				// For 3rd-party printf library specifically for embedded systems
#include "ATcommands.h"			// For AT-command-list and functions

// Define bit masks for the two types of events
#define UART1_IDLE_EVENT (1 << 0)
#define UART2_EVENT (1 << 1)

#define BUFFER_SIZE 4096

void StartWifiClick(void *argument);
void UartHandlerTask(void *argument); // Task which handles the intermediate communication between UART interfaces
void SendATCommand(char *command, int length); // Function which transmits a provided string to UART1


#endif /* INC_WIFICLICK_H_ */
