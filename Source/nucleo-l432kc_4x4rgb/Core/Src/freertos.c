/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "wifiClick.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint8_t updateLedMatrix = 0;

uint8_t newDataAvailable = 0;

uint8_t dataToPrint;

/* USER CODE END Variables */
/* Definitions for receiverTask */
osThreadId_t receiverTaskHandle;
const osThreadAttr_t receiverTask_attributes = {
  .name = "receiverTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for startWifiClickT */
osThreadId_t startWifiClickTHandle;
const osThreadAttr_t startWifiClickT_attributes = {
  .name = "startWifiClickT",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for updateScreenTimer */
osTimerId_t updateScreenTimerHandle;
const osTimerAttr_t updateScreenTimer_attributes = {
  .name = "updateScreenTimer"
};
/* Definitions for sem_printPermission */
osSemaphoreId_t sem_printPermissionHandle;
const osSemaphoreAttr_t sem_printPermission_attributes = {
  .name = "sem_printPermission"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartReceiverTask(void *argument);
void _startWifiClickTask(void *argument);
void updateScreenTimerCallback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of sem_printPermission */
  sem_printPermissionHandle = osSemaphoreNew(1, 1, &sem_printPermission_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of updateScreenTimer */
  updateScreenTimerHandle = osTimerNew(updateScreenTimerCallback, osTimerPeriodic, NULL, &updateScreenTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of receiverTask */
  receiverTaskHandle = osThreadNew(StartReceiverTask, NULL, &receiverTask_attributes);

  /* creation of startWifiClickT */
  startWifiClickTHandle = osThreadNew(_startWifiClickTask, NULL, &startWifiClickT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartReceiverTask */
/**
  * @brief  Function implementing the receiverTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartReceiverTask */

/**

    @brief Receiver task.

    This task continuously checks for new data and triggers the printing of the data on the LED matrix
    if both the updateLedMatrix flag and newDataAvailable flag are set. Once the data is printed,
    it clears the updateLedMatrix flag and sets the newDataAvailable flag.

    @param argument Pointer to the argument passed to the function (unused).
    */

void StartReceiverTask(void *argument)
{
  /* USER CODE BEGIN StartReceiverTask */
  for(;;)
  {
	  if(updateLedMatrix && newDataAvailable)
	  {
		  updateLedMatrix = 0;
		  newDataAvailable = 1;

		  printDataOnMatrix(&dataToPrint);
	  }

    osDelay(1);
  }
  /* USER CODE END StartReceiverTask */
}

/* USER CODE BEGIN Header__startWifiClickTask */
/**
* @brief Function implementing the startWifiClickT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header__startWifiClickTask */
void _startWifiClickTask(void *argument)
{
  /* USER CODE BEGIN _startWifiClickTask */
	StartWifiClick(&dataToPrint);
	osThreadExit();
  /* USER CODE END _startWifiClickTask */
}

/* updateScreenTimerCallback function */
void updateScreenTimerCallback(void *argument)
{
  /* USER CODE BEGIN updateScreenTimerCallback */
	updateLedMatrix = 1;
  /* USER CODE END updateScreenTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* TODO: @Thomas diese bei dir dann aufrufen wenn neue Daten kommen */
void NewDataAvailableCallback()
{
	newDataAvailable = 1;
}

/* USER CODE END Application */

