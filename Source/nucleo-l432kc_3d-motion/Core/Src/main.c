/* *********************** INCLUDES *********************** */

#include "main.h"
#include "cmsis_os.h"
#include "tasks.h"
#include "../../Core/MikroeSDK_3D-Motion/app.h"

/* *********************** DEFINES *********************** */

#define DATA_REFRESH_CYCLE_IN_MS	200
#define FIFTY_MS					50

/* ******************** FUNC Prototypes ****************** */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
void StartInputTask(void *argument);
void StartProcessTask(void *argument);
void StartOutputTask(void *argument);
void getNewDataCallback(void *argument);
void i2cTimerCallback(void *argument);
void fiftyMsTimerCallback(void *argument);

/* *********************** Vars and Structs *********************** */

extern volatile BOOL I2C_TIMEOUT_50MS_CNTR;
extern volatile UINT32 TIMER_1MS_FLG;
extern volatile BOOL EC_DATA_AVAIL;
extern volatile BOOL TIMER_50MS_FLG;

uint8_t state = 0;
uint8_t result;

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

osThreadId_t inputTaskHandle;
osThreadId_t processTaskHandle;
osThreadId_t outputTaskHandle;

osTimerId_t getNewDataTimerHandle;
osTimerId_t i2cTimerHandle;
osTimerId_t fiftyMsTimerHandle;

const osThreadAttr_t inputTask_attributes = {
		.name = "inputTask",
		.stack_size = 256 * 4,
		.priority = (osPriority_t) osPriorityHigh,
};

const osThreadAttr_t processTask_attributes = {
		.name = "processTask",
		.stack_size = 256 * 4,
		.priority = (osPriority_t) osPriorityNormal1,
};

const osThreadAttr_t outputTask_attributes = { .name = "outputTask",
		.stack_size = 256 * 4, .priority = (osPriority_t) osPriorityNormal2, };

const osTimerAttr_t getNewDataTimer_attributes = { .name = "getNewDataTimer" };

const osTimerAttr_t i2cTimer_attributes = { .name = "i2cTimer" };

const osTimerAttr_t fiftyMsTimer_attributes = { .name = "fiftyMsTimer" };

/* *********************** Main *********************** */

int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();

	/* Init scheduler */
	osKernelInitialize();

	/* Create the timer(s) */
	getNewDataTimerHandle = osTimerNew(getNewDataCallback, osTimerPeriodic,
			NULL, &getNewDataTimer_attributes);
	i2cTimerHandle = osTimerNew(i2cTimerCallback, osTimerPeriodic, NULL,
			&i2cTimer_attributes);
	fiftyMsTimerHandle = osTimerNew(fiftyMsTimerCallback, osTimerPeriodic, NULL,
			&fiftyMsTimer_attributes);

	/* Create the thread(s) */
	inputTaskHandle = osThreadNew(StartInputTask, NULL, &inputTask_attributes);
	processTaskHandle = osThreadNew(StartProcessTask, NULL,
			&processTask_attributes);
	outputTaskHandle = osThreadNew(StartOutputTask, NULL,
			&outputTask_attributes);

	/* Start the Timers */
	osTimerStart(fiftyMsTimerHandle, FIFTY_MS);
	osTimerStart(getNewDataTimerHandle, DATA_REFRESH_CYCLE_IN_MS);

	/* Start scheduler */
	osKernelStart();

	while (1) {
		/* We should never get here */
	}

}



/* *********************** Task Functions *********************** */

void StartInputTask(void *argument) {
	Wake_signal();
	result = VREG_init();
	for (;;) {
		Wake_signal();
		_rawGyro();  // Note: array has 7 elements, not 8
		osDelay(3000);
	}

}

void StartProcessTask(void *argument) {
	for (;;) {
		osDelay(1000);
	}
}

void StartOutputTask(void *argument) {
	for (;;) {
		osDelay(1000);
	}
}


/* *********************** Callbacks & ISRs *********************** */

void getNewDataCallback(void *argument) {

}

void i2cTimerCallback(void *argument) {
	I2C_TIMEOUT_50MS_CNTR = I2C_TIMEOUT_50MS_CNTR;
	if (I2C_TIMEOUT_50MS_CNTR >= I2_TIMEOUT_PERIOD) { // looks like i2c taking too long
		StopI2CTimer();                                        // turn off timer
		error_handler("i2c ", 0, I2C_TIMEOUT_ERR); // displays to LCD and uart **does not return***
	}
}

void fiftyMsTimerCallback(void *argument) {
	TIMER_50MS_FLG = TRUE;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_3) {

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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM16) {
		HAL_IncTick();
	}
}


/* *********************** Error Handler STuff *********************** */

void Error_Handler(void) {
	__disable_irq();
	while (1) {
	}
}


/* *********************** CUBE INIT STUFF *********************** */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	HAL_RCCEx_EnableMSIPLLMode();
}

static void MX_I2C1_Init(void) {
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
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
}

static void MX_USART2_UART_Init(void) {
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
}

static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

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
