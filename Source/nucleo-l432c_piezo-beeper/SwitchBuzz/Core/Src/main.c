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
#include "stdio.h"
#include "wifiClick.h"
#include "printf.h"
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
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;

/* Definitions for t1_Buzzer */
osThreadId_t t1_BuzzerHandle;
const osThreadAttr_t t1_Buzzer_attributes = {
  .name = "t1_Buzzer",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for t2_Listener */
osThreadId_t t2_ListenerHandle;
const osThreadAttr_t t2_Listener_attributes = {
  .name = "t2_Listener",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t3_Wifi_Listene */
osThreadId_t t3_Wifi_ListeneHandle;
const osThreadAttr_t t3_Wifi_Listene_attributes = {
  .name = "t3_Wifi_Listene",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for t4_Wifi_Starter */
osThreadId_t t4_Wifi_StarterHandle;
const osThreadAttr_t t4_Wifi_Starter_attributes = {
  .name = "t4_Wifi_Starter",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *																   *
 *  Clock Frequency: 80MHz										   *
 *  PWM Transmission Time for one 1 Bit: 1.25us					   *
 * 	PWM Frequency: 1/1.25us = 800kHz							   *
 * 	TIM1 Auto Reload Register: 800kHz/800kHz=100				   *
 * 	TIM1 Capture Compare Register: 57/31						   *
 *																   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 	Received Data Codes
 * 	==================
 * 	a = middle --> 500
 * 	b = out of scope --> 5.000
 * 	c = shake detected --> 10.000
 * 	d = light shift lo --> 1000
 * 	e = light shift lu --> 1000 - 42*1
 * 	f = light shift ro --> 1000
 * 	g = light shift ru --> 1000 - 42*1
 * 	h = strong shift r1c1 --> 2000
 * 	i = strong shift r1c2 --> 2000 - 84*1
 * 	j = strong shift r1c3 --> 2000 - 84*2
 * 	k = strong shift r1c4 --> 2000 - 84*3
 * 	l = strong shift r2c1 --> 2000 - 84*4
 * 	m = strong shift r2c4 --> 2000 - 84*5
 * 	n = strong shift r3c1 --> 2000 - 84*4
 * 	o = strong shift r3c4 --> 2000 - 84*5
 * 	p = strong shift r4c1 --> 2000
 * 	q = strong shift r4c2 --> 2000 - 84*1
 * 	r = strong shift r4c3 --> 2000 - 84*2
 * 	s = strong shift r4c4 --> 2000 - 84*3
 *
 */
/**
 * rx_x variables are used in the f4_set_TIM_Register() function, which is called in the START_Buzzer task
 * based on the fields of the 4x4 RGB-Click, the rx_x variables are used to set an according sound by changing the ARR-value of TIM
 * the sounds between the levels "middle - light shift - strong shift" are one octave apart. The sounds within a level vary by half-tone steps
 */
const int rx_a = 500;
const int rx_b = 5000;
const int rx_c = 10000;
const int rx_d = 1000;
const int rx_e = 1000 - (42*1);
const int rx_f = 1000;
const int rx_g = 1000 - (42*1);
const int rx_h = 2000;
const int rx_i = 2000 - 84*1;
const int rx_j = 2000 - 84*2;
const int rx_k = 2000 - 84*3;
const int rx_l = 2000 - 84*4;
const int rx_m = 2000 - 84*5;
const int rx_n = 2000 - 84*4;
const int rx_o = 2000 - 84*5;
const int rx_p = 2000;
const int rx_q = 2000 - 84*1;
const int rx_r = 2000 - 84*2;
const int rx_s = 2000 - 84*3;

/**
 * @param rx_wifi: will be written during runtime by the Wifi_Click.
 * It is the comparison variable used in the switch-case block of the t3_Wifi_Listener task
 */
volatile char rx_wifi = 0;
/**
 * @param rx_uart: will receive values via UART during runtime.
 * It is the comparison variable used in the switch-case block of the t1_Buzzer task
 */
volatile char rx_uart = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
void START_Buzzer(void *argument);
void START_Listener(void *argument);
void START_Wifi_Listener(void *argument);
void START_Wifi_Starter(void *argument);

/* USER CODE BEGIN PFP */
void f4_set_TIM_Register(int x);
void f4_send_UART();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of t1_Buzzer */
  t1_BuzzerHandle = osThreadNew(START_Buzzer, NULL, &t1_Buzzer_attributes);

  /* creation of t2_Listener */
  t2_ListenerHandle = osThreadNew(START_Listener, NULL, &t2_Listener_attributes);

  /* creation of t3_Wifi_Listene */
  t3_Wifi_ListeneHandle = osThreadNew(START_Wifi_Listener, NULL, &t3_Wifi_Listene_attributes);

  /* creation of t4_Wifi_Starter */
  t4_Wifi_StarterHandle = osThreadNew(START_Wifi_Starter, NULL, &t4_Wifi_Starter_attributes);

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
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 32;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 2000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500-1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * @brief Sets the sound of the Buzz-Click by writing into the Auto-Reload-Register and the Captcher-Compare-Register of TIM1.
 * if the parameter value x = 0, the Buzz-Click will be silenced
 * @param x is one of the rx_x values
 */
void f4_set_TIM_Register(int x)
{
	if(x != NULL)
	{
		if(x == 0){
			TIM1 -> ARR = 0;
			TIM1 -> CCR1 = 0;
		} else
		{
			TIM1 -> ARR = x;
			TIM1 -> CCR3 = (x/2);
		}
	}

}
/**
 * @brief Sends back the typed in value from the UART-Monitor
 * @retval None
 */
void f4_send_UART()
{
	printf("You pressed: %c\n", rx_uart);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_START_Buzzer */
/**
  * @brief  Function implementing the t1_Buzzer thread. Starts the PWM and calls the f4_set_TIM_Register() function to set the sound of the Buzz-Click.
  * If any other values than defined by the switch-case are written into rx_uart, the Buzz-Click will be silenced
  * If the user enters 'X', the t1_Buzzer task will kill itself, in order not to interfere with the t3_Wifi_Listener task
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_START_Buzzer */
void START_Buzzer(void *argument)
{
  /* USER CODE BEGIN 5 */
	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3) != HAL_OK){
		printf("PWM didn't start. Try to refresh the project")
	}
	printf("Enter Value a-s\nIf X is entered the task will end.\n");
  /* Infinite loop */
  for(;;)
  {
    switch(rx_uart)
    {
    case 'a': f4_set_TIM_Register(rx_a); continue;
    case 'b': f4_set_TIM_Register(rx_b); continue;
    case 'c': f4_set_TIM_Register(rx_c); continue;
    case 'd': f4_set_TIM_Register(rx_d); continue;
    case 'e': f4_set_TIM_Register(rx_e); continue;
    case 'f': f4_set_TIM_Register(rx_f); continue;
    case 'g': f4_set_TIM_Register(rx_g); continue;
    case 'h': f4_set_TIM_Register(rx_h); continue;
    case 'i': f4_set_TIM_Register(rx_i); continue;
    case 'j': f4_set_TIM_Register(rx_j); continue;
    case 'k': f4_set_TIM_Register(rx_k); continue;
    case 'l': f4_set_TIM_Register(rx_l); continue;
    case 'm': f4_set_TIM_Register(rx_m); continue;
    case 'n': f4_set_TIM_Register(rx_n); continue;
    case 'o': f4_set_TIM_Register(rx_o); continue;
    case 'p': f4_set_TIM_Register(rx_p); continue;
    case 'q': f4_set_TIM_Register(rx_q); continue;
    case 'r': f4_set_TIM_Register(rx_r); continue;
    case 's': f4_set_TIM_Register(rx_s); continue;
    case 'X': osThreadExit();
    default: f4_set_TIM_Register(0); osDelay(1000);
    }

  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_START_Listener */
/**
* @brief Function implementing the t2_Listener thread. Receives (hopefully) character values from the UART-Monitor and writes it into the rx_uart variable.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_START_Listener */
void START_Listener(void *argument)
{
  /* USER CODE BEGIN START_Listener */
uint8_t buff;
f4_send_UART(rx_uart);
  for(;;)
  {
    osDelay(1);
// if(HAL_UART_Receive_IT(&huart2, &buff, 1)))
    if(HAL_UART_Receive_IT(&huart2, &buff, 1))
    	{
    	rx_uart = (char)buff;
    	}

  }
  /* USER CODE END START_Listener */
}

/* USER CODE BEGIN Header_START_Wifi_Listener */
/**
* @brief Function implementing the t3_Wifi_Listene thread. Calls the f4_set_TIM_Register() function according to the received values in rx_wifi.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_START_Wifi_Listener */
void START_Wifi_Listener(void *argument)
{
  /* USER CODE BEGIN START_Wifi_Listener */
  /* Infinite loop */
  for(;;)
  {

    osDelay(1);
    switch(rx_wifi)
       {
       case 'a': f4_set_TIM_Register(rx_a); continue;
       case 'b': f4_set_TIM_Register(rx_b); continue;
       case 'c': f4_set_TIM_Register(rx_c); continue;
       case 'd': f4_set_TIM_Register(rx_d); continue;
       case 'e': f4_set_TIM_Register(rx_e); continue;
       case 'f': f4_set_TIM_Register(rx_f); continue;
       case 'g': f4_set_TIM_Register(rx_g); continue;
       case 'h': f4_set_TIM_Register(rx_h); continue;
       case 'i': f4_set_TIM_Register(rx_i); continue;
       case 'j': f4_set_TIM_Register(rx_j); continue;
       case 'k': f4_set_TIM_Register(rx_k); continue;
       case 'l': f4_set_TIM_Register(rx_l); continue;
       case 'm': f4_set_TIM_Register(rx_m); continue;
       case 'n': f4_set_TIM_Register(rx_n); continue;
       case 'o': f4_set_TIM_Register(rx_o); continue;
       case 'p': f4_set_TIM_Register(rx_p); continue;
       case 'q': f4_set_TIM_Register(rx_q); continue;
       case 'r': f4_set_TIM_Register(rx_r); continue;
       case 's': f4_set_TIM_Register(rx_s); continue;
       default: f4_set_TIM_Register(0); osDelay(1000);
       }
  }
  /* USER CODE END START_Wifi_Listener */
}

/* USER CODE BEGIN Header_START_Wifi_Starter */
/**
* @brief Function implementing the t4_Wifi_Starter thread. Starts the Wifi-Click that writes values into the rx_wifi variable.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_START_Wifi_Starter */
void START_Wifi_Starter(void *argument)
{
  /* USER CODE BEGIN START_Wifi_Starter */
	StartWifiClick(argument);
	osThreadExit();
  /* USER CODE END START_Wifi_Starter */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
