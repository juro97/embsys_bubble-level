/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */
extern volatile BOOL EC_DATA_AVAIL;
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MOTION_WAKE_PIN_GPIO_Port, MOTION_WAKE_PIN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MOTION_WAKE_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(MOTION_WAKE_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MOTION_EXTI3_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MOTION_EXTI3_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = MOTION_RESET_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(MOTION_RESET_PIN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

}

/* USER CODE BEGIN 2 */

/**
 * @brief: Checks if the Interrupt came from the Click Module and sets the Data Available Flag.
 * 			Sets the Interrupt to fire on opposite Signal-Edge.
 * @param: The Pin from where the Interrupt came from
 * TODO: Directly Write to the Registers without using the HAL functions
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Check if the INT Source is our Data Available PIN */
    if(GPIO_Pin == MOTION_EXTI3_PIN_Pin)
    {
    	/*If no data available, but INT Line was driven LOW */
        if (!EC_DATA_AVAIL)
        {
        	/* INT1 Edge configured to interrupt on rising edge (wait for end of data) */
            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;

            /*Toggle EC_DATA_AVAIL flag to notify data received */
            EC_DATA_AVAIL = TRUE;
        }
        /* if data was available and INT line got driven HIGH */
        else
        {
        	/* INT1 Edge configured to interrupt on falling edge (data is no longer available) */
            GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;

            /*Toggle EC_DATA_AVAIL flag to notify that there is no new data rn */
            EC_DATA_AVAIL = FALSE;
        }

		GPIO_InitStruct.Pin = MOTION_EXTI3_PIN_Pin;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        /* Re-Initialize the Portpin to Trigger on falling/rising Edge */
        HAL_GPIO_Init(MOTION_EXTI3_PIN_GPIO_Port, &GPIO_InitStruct);
    }
}

/* USER CODE END 2 */
