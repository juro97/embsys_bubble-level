#include "app.h"
#include "main.h"
#include "cmsis_os.h"

extern volatile BOOL TIMER_1MS_FLG;
extern volatile BOOL EC_DATA_AVAIL; // HIDI2_HOST_INT indicates EC data available

extern TIM_HandleTypeDef htim2;

volatile UINT32 I2C_TIMEOUT_1MS_CNTR = 0;


void StopI2CTimer()
{
  HAL_TIM_Base_Stop_IT(&htim2);
}

void StartI2CTimer()
{
  I2C_TIMEOUT_1MS_CNTR = 0;
  HAL_TIM_Base_Start_IT(&htim2);
}
