
/**
 * I2C System Functions for Reading, Writing to the 3D Motion Module
 */
#include "main.h"
#include "../MikroeSDK_3D-Motion/app.h"

volatile uint16_t status = 0;

extern I2C_HandleTypeDef hi2c1;

extern TIM_HandleTypeDef htim2;


HAL_StatusTypeDef i2c_cmd_WrRd(uint8_t ucCmd, uint8_t ucBytes_wr, uint8_t *ucData_wr, uint16_t usBytes_rd, uint8_t *ucData_rd, uint8_t bAdjust)
{
  HAL_StatusTypeDef status;

  switch (ucCmd)
  {
  case WRITE:
    // Send a START condition and then the slave address followed by the WRITE bit
    status = HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1, (uint16_t)SLAVE_ADDR<<1, ucData_wr, ucBytes_wr, I2C_LAST_FRAME);
    if (status != HAL_OK)
      return status;
    break;

  case READ:
    // Send a START condition and then the slave address followed by the READ bit
    status = HAL_I2C_Master_Sequential_Receive_IT(&hi2c1, (uint16_t)SLAVE_ADDR<<1, ucData_rd, usBytes_rd, I2C_LAST_FRAME);
    if (status != HAL_OK)
      return status;
    break;

  case WR_RD:
    // Send a START condition and then the slave address followed by the WRITE bit
    status = HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1, (uint16_t)SLAVE_ADDR<<1, ucData_wr, ucBytes_wr, I2C_FIRST_AND_NEXT_FRAME);
    if (status != HAL_OK)
      return status;
    // Send a RESTART condition and then the slave address followed by the READ bit
    status = HAL_I2C_Master_Sequential_Receive_IT(&hi2c1, (uint16_t)SLAVE_ADDR<<1, ucData_rd, usBytes_rd, I2C_LAST_FRAME);
    if (status != HAL_OK)
      return status;
    break;
  }

  return HAL_OK;
}

