/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../Core/MikroeSDK_3D-Motion/app.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define I2C_TIMEOUT_IN_MS			1
#define DATA_REFRESH_CYCLE_IN_MS	200
#define ONE_MS						1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* Definitions for inputTask */
osThreadId_t inputTaskHandle;
const osThreadAttr_t inputTask_attributes = { .name = "inputTask", .stack_size =
		256 * 4, .priority = (osPriority_t) osPriorityHigh, };
/* Definitions for processTask */
osThreadId_t processTaskHandle;
const osThreadAttr_t processTask_attributes = { .name = "processTask",
		.stack_size = 256 * 4, .priority = (osPriority_t) osPriorityNormal1, };
/* Definitions for outputTask */
osThreadId_t outputTaskHandle;
const osThreadAttr_t outputTask_attributes = { .name = "outputTask",
		.stack_size = 256 * 4, .priority = (osPriority_t) osPriorityNormal2, };
/* Definitions for getNewDataTimer */
osTimerId_t getNewDataTimerHandle;
const osTimerAttr_t getNewDataTimer_attributes = { .name = "getNewDataTimer" };
/* Definitions for i2cTimer */
osTimerId_t i2cTimerHandle;
const osTimerAttr_t i2cTimer_attributes = { .name = "i2cTimer" };
/* Definitions for oneMsTimer */
osTimerId_t oneMsTimerHandle;
const osTimerAttr_t oneMsTimer_attributes = { .name = "oneMsTimer" };
/* USER CODE BEGIN PV */

extern volatile BOOL I2C_TIMEOUT_1MS_CNTR;
extern volatile UINT32 TIMER_1MS_FLG;
extern volatile BOOL EC_DATA_AVAIL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
void StartInputTask(void *argument);
void StartProcessTask(void *argument);
void StartOutputTask(void *argument);
void getNewDataCallback(void *argument);
void i2cTimerCallback(void *argument);
void oneMsTimerCallback(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t state = 0;
uint8_t result;

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* Create the timer(s) */
	/* creation of getNewDataTimer */
	getNewDataTimerHandle = osTimerNew(getNewDataCallback, osTimerPeriodic,
			NULL, &getNewDataTimer_attributes);

	/* creation of i2cTimer */
	i2cTimerHandle = osTimerNew(i2cTimerCallback, osTimerPeriodic, NULL,
			&i2cTimer_attributes);

	/* creation of oneMsTimer */
	oneMsTimerHandle = osTimerNew(oneMsTimerCallback, osTimerPeriodic, NULL,
			&oneMsTimer_attributes);

	/* USER CODE BEGIN RTOS_TIMERS */

	/* for final version make this beautiful */
	/* I2C Failure Timer is stopped/started in interrupt.c file */

	if (osTimerStart(oneMsTimerHandle, ONE_MS) != osOK) {
		/* Timer Could not be started */while (1)
			;
	}

	if (osTimerStart(getNewDataTimerHandle, DATA_REFRESH_CYCLE_IN_MS) != osOK) {
		/* Timer Could not be started */while (1)
			;
	}
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of inputTask */
	inputTaskHandle = osThreadNew(StartInputTask, NULL, &inputTask_attributes);

	/* creation of processTask */
	processTaskHandle = osThreadNew(StartProcessTask, NULL,
			&processTask_attributes);

	/* creation of outputTask */
	outputTaskHandle = osThreadNew(StartOutputTask, NULL,
			&outputTask_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();
	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	/* In Case we come to this section, delete all ressources */
	osTimerDelete(getNewDataTimerHandle);
	osTimerDelete(oneMsTimerHandle);
	osTimerDelete(i2cTimerHandle);

	osThreadTerminate(inputTaskHandle);
	osThreadTerminate(processTaskHandle);
	osThreadTerminate(outputTaskHandle);

	/*TODO:  some more stuff has to be done here, but later .. */

	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE
			| RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}

	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00702991;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, MOTION_CLICK_WAKE_Pin | RGB_LED_GREEN_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : MOTION_CLICK_WAKE_Pin RGB_LED_GREEN_Pin */
	GPIO_InitStruct.Pin = MOTION_CLICK_WAKE_Pin | RGB_LED_GREEN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : MOTION_CLICK_INTERRUPT_Pin */
	GPIO_InitStruct.Pin = MOTION_CLICK_INTERRUPT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(MOTION_CLICK_INTERRUPT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD3_Pin */
	GPIO_InitStruct.Pin = LD3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	// Check if the interrupt comes from PA3
	if (GPIO_Pin == GPIO_PIN_3) {
		// Toggle the LED

		GPIO_PinState PinState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);

		if (PinState == GPIO_PIN_SET) {
			// If the pin is set, the interrupt was caused by a rising edge
			EC_DATA_AVAIL = TRUE;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		} else {
			// If the pin is reset, the interrupt was caused by a falling edge
			EC_DATA_AVAIL = FALSE;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		}

	}
}

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartInputTask */
/**
 * @brief  Function implementing the inputTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInputTask */
void StartInputTask(void *argument) {
	/* USER CODE BEGIN 5 */

	/* TODO: REWRITE FOR OUR CONTROLLER and delete unnecessary stuff */

	Wake_signal();
	result = VREG_init();                // Initialize VREG registers

	/* function pointer to functions getting specific sensor data */
	/*void (*fp_SensorDatas[])() = {
	 _rawAccel, _rawGyro, _rawMagnet, _accel, _compass, _inclin, _orient
	 };*/

	//static size_t i = 0;
	/* Infinite loop */
	for (;;) {
		Wake_signal();
		_rawGyro();  // Note: array has 7 elements, not 8
		osDelay(3000);
	}

	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartProcessTask */
/**
 * @brief Function implementing the processTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartProcessTask */
void StartProcessTask(void *argument) {
	/* USER CODE BEGIN StartProcessTask */
	/* Infinite loop */
	for (;;)
	{

		osDelay(1000);
	}
	/* USER CODE END StartProcessTask */
}

/* USER CODE BEGIN Header_StartOutputTask */
/**
 * @brief Function implementing the outputTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartOutputTask */
void StartOutputTask(void *argument) {
	/* USER CODE BEGIN StartOutputTask */

	/* Infinite loop */
	for (;;) {

		osDelay(1000);
	}
	/* USER CODE END StartOutputTask */
}

/* getNewDataCallback function */
void getNewDataCallback(void *argument) {
	/* USER CODE BEGIN getNewDataCallback */

	/* USER CODE END getNewDataCallback */
}

/* i2cTimerCallback function */
void i2cTimerCallback(void *argument) {
	/* USER CODE BEGIN i2cTimerCallback */
	I2C_TIMEOUT_1MS_CNTR++;

	if (I2C_TIMEOUT_1MS_CNTR >= I2_TIMEOUT_PERIOD) { // looks like i2c taking too long
		StopI2CTimer();                                        // turn off timer
		error_handler("i2c ", 0, I2C_TIMEOUT_ERR); // displays to LCD and uart **does not return***
	}
	/* USER CODE END i2cTimerCallback */
}

/* oneMsTimerCallback function */
void oneMsTimerCallback(void *argument) {
	/* USER CODE BEGIN oneMsTimerCallback */
	TIMER_1MS_FLG = 1;
	/* USER CODE END oneMsTimerCallback */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM16 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM16) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
