#include "app.h"
#include "main.h"
#include "cmsis_os.h"

extern osTimerId_t i2cTimerHandle;
extern TIM_HandleTypeDef htim2;

volatile UINT32 I2C_TIMEOUT_50MS_CNTR = 0;

/* Every 50ms the Software Timer gets triggered.
 * Within the ISR the Counter Value gets incremented by 50.
 * On 250ms, the i2c connection send an error.
 */

void StopI2CTimer()
{
  osTimerStop(i2cTimerHandle);
}

void StartI2CTimer()
{
  /* Reset the Counter Value */
	I2C_TIMEOUT_50MS_CNTR = 0;

  /* Start the Software Timer and set 50ms as Overflow Value */
  osTimerStart(i2cTimerHandle, 50);
}
