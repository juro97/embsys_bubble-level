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
#include "mpu9dof.h"
#include "wifiClick.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define NDEBUG

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

const uint8_t GYRO = 1;
const uint8_t ACCEL = 2;
const uint8_t RAW = 1;
const uint8_t PROCESSED = 2;
bool wifiIsReady = false;

/* USER CODE END Variables */
/* Definitions for readDataTask */
osThreadId_t readDataTaskHandle;
const osThreadAttr_t readDataTask_attributes = {
  .name = "readDataTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sendDataTask */
osThreadId_t sendDataTaskHandle;
const osThreadAttr_t sendDataTask_attributes = {
  .name = "sendDataTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for startWifiClickT */
osThreadId_t startWifiClickTHandle;
const osThreadAttr_t startWifiClickT_attributes = {
  .name = "startWifiClickT",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for reportDataQueue */
osMessageQueueId_t reportDataQueueHandle;
const osMessageQueueAttr_t reportDataQueue_attributes = {
  .name = "reportDataQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartReadDataTask(void *argument);
void startSendDataTask(void *argument);
void _startWifiClickTask(void *argument);

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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of reportDataQueue */
  reportDataQueueHandle = osMessageQueueNew (64, sizeof(uint16_t), &reportDataQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of readDataTask */
  //readDataTaskHandle = osThreadNew(StartReadDataTask, NULL, &readDataTask_attributes);

  /* creation of sendDataTask */
  sendDataTaskHandle = osThreadNew(startSendDataTask, NULL, &sendDataTask_attributes);

  /* creation of startWifiClickT */
  startWifiClickTHandle = osThreadNew(_startWifiClickTask, NULL, &startWifiClickT_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartReadDataTask */
/**
  * @brief  Function implementing the readDataTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartReadDataTask */
void StartReadDataTask(void *argument)
{
  /* USER CODE BEGIN StartReadDataTask */
	printf("***------------- 9 DOF IMU Click ------------***\r\n");


	while (!wifiIsReady){
		osThreadYield();
	}

    /* config the 9dof module */
    mpu9dof_default_cfg();

    /* calibrate the module */
    mpu9dof_calibrate_module();

    ReportData *data;

  /* Infinite loop */
  for(;;)
  {
	  /* read the gyro and accel values */
	  mpu9dof_read(&GYRO);
	  mpu9dof_read(&ACCEL);

	  /* dave the processed sensor data */
	  data = (ReportData *) mpu9dof_process();

	  /* report the data to the sending task */
	  osMessageQueuePut(reportDataQueueHandle, &data, 0U, 0U);

	  osDelay(100);
  }
  /* USER CODE END StartReadDataTask */
}

/* USER CODE BEGIN Header_startSendDataTask */
/**
* @brief Function implementing the sendDataTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_startSendDataTask */
void startSendDataTask(void *argument)
{
  /* USER CODE BEGIN startSendDataTask */
	char at_cmd_buffer[AT_CMD_BUFFER_SIZE] = {0};
	char msg[25] = {0};
	/* transfer interval in ms */
	/* if transfer interval is smaller than sensor data rate, the que is blocking */
#ifdef NDEBUG
	const uint8_t RAW_DATA = 1;
	const uint8_t OFFSET_VALUES = 2;
	const uint8_t CALIBRATED_VALUES = 3;
	const uint8_t PROCESSED_DATA = 4;
#endif

	const uint8_t TRANSFER_INTERVAL = 100;

	ReportData data;

	while (!wifiIsReady){
		osThreadYield();
	}

  for(;;)
  {
	if(osMessageQueueGet(reportDataQueueHandle, &data, 0U, 0U) == osOK || true)
	{
		sprintf(msg,"%d,%d,%d\n\r",data.roll, data.pitch, data.shake);
		at_set_command(at_cmd_buffer, SendATCommand, AT_IP_Send, "0,%u", sizeof(msg), 80);
		osDelay(10);
		SendATCommand(msg,sizeof(msg));
		osDelay(10);

#ifdef NDEBUG
	  /* delete logging for final version */
	  mpu9dof_log_data(&PROCESSED_DATA);
#endif

	} else {
		osThreadYield();
	}

    osDelay(TRANSFER_INTERVAL);
  }
  /* USER CODE END startSendDataTask */
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
	StartWifiClick(argument);
	wifiIsReady = true;
	osThreadExit();
  /* USER CODE END _startWifiClickTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

