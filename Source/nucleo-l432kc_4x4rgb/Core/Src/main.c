/**************************************************************************
 ******************************** INCLUDES ********************************
 **************************************************************************/
#include "main.h"
#include "cmsis_os.h"

#include "clk_4x4_rgb.h"


/**************************************************************************
 ****************************** DEFINES ***********************************
 **************************************************************************/

#define NEW_DATA_INTERVAL 	200


/**************************************************************************
 ************************** Function Prototypes ***************************
 **************************************************************************/

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
void StartConServerTask(void *argument);
void newDataTimerCallback(void *argument);


/**************************************************************************
 ***************************** VARS & TYPEDEFS ****************************
 **************************************************************************/

bool getNewData = false;


TIM_HandleTypeDef htim1;
DMA_HandleTypeDef hdma_tim1_ch1;
UART_HandleTypeDef huart2;


osThreadId_t ConServerTaskHandle;
osThreadId_t OutputDataTaskHandle;

osTimerId_t newDataTimerHandle;

osSemaphoreId_t sem_printPermissionHandle;


const osThreadAttr_t ConServerTask_attributes = {
		.name = "ConServerTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityHigh,
};

const osThreadAttr_t OutputDataTask_attributes = {
		.name = "OutputDataTaskHandle",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};

const osTimerAttr_t newDataTimer_attributes = {
		.name = "newDataTimer"
};

const osSemaphoreAttr_t sem_printPermission_attributes = {
		.name = "sem_printPermission"
};



/**************************************************************************
 ***************************** MAIN FUNC **********************************
 **************************************************************************/

int main(void)
{
	/* Initial System Configuration */
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();
	MX_TIM1_Init();

	/* Init scheduler */
	osKernelInitialize();

	/* Create the thread(s) */
	ConServerTaskHandle = osThreadNew(StartConServerTask, NULL, &ConServerTask_attributes);

	/* Create the thread(s) */
	ConServerTaskHandle = osThreadNew(StartConServerTask, NULL, &ConServerTask_attributes);


	/* Create the semaphores(s) */
	sem_printPermissionHandle = osSemaphoreNew(1, 1, &sem_printPermission_attributes);

	/* Create the timer(s) */
	newDataTimerHandle = osTimerNew(newDataTimerCallback, osTimerPeriodic, NULL, &newDataTimer_attributes);


	/* Start scheduler */
	osKernelStart();


	/* We should never get here as control is now taken by the scheduler */
	while (1)
	{
		;
	}
}


/**************************************************************************
 ******************************* TASK Functions ***************************
 **************************************************************************/


void StartConServerTask(void *argument)
{
	for(;;)
	{
		if(serverReachable() == true)
		{
			if(connectToServer() == true)
			{
				/* Yield this task so that OutPutDataTask can Start */
				osThreadYield();
			}
		}

		/* Block and wait for 50ms */
		HAL_Delay(50);
	}
}


void StartOutputDataTask(void *argument)
{
	const char RESET = 'Z';
	char data = RESET;

	/* turn off all leds */
	printDataOnMatrix(&RESET);

	/* start the timer for new data interval */
    if (osTimerStart (newDataTimerHandle, NEW_DATA_INTERVAL) != osOK)
    {
    	Error_Handler();
    }

    /* FOREVER LOOP */
	for (;;)
	{
		if(getNewData == true)
		{
			if(getMotionDataFromServer(&data) == true)
			{
				/* turn off all leds */
				printDataOnMatrix(&RESET);

				/* print new led pattern */
				printDataOnMatrix(&data);
			}
			else
			{
				/* Yield this task so that ConnectToServerTask can start*/
				osTimerStop(newDataTimerHandle);
				osThreadYield();
			}
		}
		osDelay(100);
	}
}


/**************************************************************************
 ******************************* ERROR HANDLER ****************************
 **************************************************************************/

void Error_Handler(void)
{
	__disable_irq();

	while (1)
	{
		;
	}
}


/**************************************************************************
 ******************************* ISR & CB ***************************
 **************************************************************************/

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	// Stop DMA
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);

	// Give Back Semaphore
	osSemaphoreRelease(sem_printPermissionHandle);
}


void newDataTimerCallback(void *argument)
{
	getNewData = true;
}



/**************************************************************************
 ******************************* HAL GENERATED INIT STUFF******************
 **************************************************************************/

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_MSI;
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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RCCEx_EnableMSIPLLMode();
}

static void MX_TIM1_Init(void)
{
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 100;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
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
	sConfigOC.Pulse = 100;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)!= HAL_OK)
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
	HAL_TIM_MspPostInit(&htim1);

}

static void MX_USART2_UART_Init(void)
{
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
}

static void MX_DMA_Init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = LD3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM16)
	{
		HAL_IncTick();
	}
}
