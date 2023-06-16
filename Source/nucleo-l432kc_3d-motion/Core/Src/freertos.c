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
extern volatile UINT32 I2C_TIMEOUT_50MS_CNTR;
extern volatile BOOL TIMER_10MS_FLG;

/* USER CODE END Variables */
/* Definitions for getDataTask */
osThreadId_t getDataTaskHandle;
const osThreadAttr_t getDataTask_attributes = {
  .name = "getDataTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for timeBaseTimer */
osTimerId_t timeBaseTimerHandle;
const osTimerAttr_t timeBaseTimer_attributes = {
  .name = "timeBaseTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartGetDataTask(void *argument);
void timeBaseCallback(void *argument);

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of timeBaseTimer */
  timeBaseTimerHandle = osTimerNew(timeBaseCallback, osTimerPeriodic, NULL, &timeBaseTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of getDataTask */
  getDataTaskHandle = osThreadNew(StartGetDataTask, NULL, &getDataTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartGetDataTask */
/**
 * @brief  Function implementing the getDataTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGetDataTask */
void StartGetDataTask(void *argument)
{
  /* USER CODE BEGIN StartGetDataTask */
	size_t state = 0;

	printf("\r\n--3D-Motion-Click--\r\n");

	/* Send Reset Signal to the Module */
	Reset_signal();
	HAL_Delay(1000);

	/* Send Wake Signal to the Module */
	Wake_signal();
	HAL_Delay(1000);

	  HAL_NVIC_SetPriority(EXTI3_IRQn, 7, 0);
	  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	/* Initialize the Virtual Register Bank */
	while(VREG_init())
	{
		printf("Init-Error\r\n");
		Reset_signal();
		HAL_Delay(1000);
		Wake_signal();
		HAL_Delay(1000);
	};
	printf("Init-OK\r\n");

	/* Infinite loop */
	for (;;)
	{
		/* TODO: Connection to Module Works, but Data Retrieval does NOT */
		_gyro();
		HAL_Delay(100);

		/* TODO: Think of a nice way to store and transmit the data via the WIFI ESP */
		/* Option 1: Store the Data in a Ring Buffer, and other Task sends it via function Call to WiFi Library => N: Might miss some old Data due to Buffer Overflow*/
		/* Option 2: Simply Call the Transfer Function at the end of every new Data => N: Might miss some new data due to Blocking */
	}
  /* USER CODE END StartGetDataTask */
}

/* timeBaseCallback function */
void timeBaseCallback(void *argument)
{
  /* USER CODE BEGIN timeBaseCallback */
	TIMER_10MS_FLG = TRUE;
  /* USER CODE END timeBaseCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

