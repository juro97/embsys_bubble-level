#include "../MikroeSDK_3D-Motion/app.h"
#include "stm32l4xx_hal.h"

extern volatile BOOL TIMER_1MS_FLG;
extern volatile BOOL EC_DATA_AVAIL;
extern TIM_HandleTypeDef htim2;
volatile UINT32 I2C_TIMEOUT_1MS_CNTR = 0;

// Interrupt pin handler, assuming INT1 is connected to PA3
void EXTI3_IRQHandler(void)
{
    // Check if EXTI line interrupt detected
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
        HAL_GPIO_EXTI_Callback(GPIO_PIN_3);
    }
}

// EXTI line detection callbacks
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == GPIO_PIN_3) // if the interrupt comes from INT1 (PA3)
    {
        if (!EC_DATA_AVAIL)
        {
            EC_DATA_AVAIL = TRUE;
        }
        else
        {
            EC_DATA_AVAIL = FALSE;
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2) // if the interrupt comes from TIM2
    {
        TIMER_1MS_FLG = TRUE;
    }
}

void InitTimer()
{
    TIM_HandleTypeDef htim2;

    __TIM2_CLK_ENABLE();

    // Configure the timer
    htim2.Instance = TIM2;
    htim2.Init.Period = 10000 - 1; // 1ms time base
    htim2.Init.Prescaler = HAL_RCC_GetPCLK1Freq() / 1000000 - 1; // 1MHz counter clock
    htim2.Init.ClockDivision = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&htim2);

    // Enable the timer interrupts
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    // Start the timer
    HAL_TIM_Base_Start_IT(&htim2);
}
