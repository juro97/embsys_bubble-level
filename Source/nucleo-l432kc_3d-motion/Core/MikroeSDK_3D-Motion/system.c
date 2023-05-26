#include "main.h"
#include "cmsis_os.h"
#include "app.h"

/** Wake_signal
* @note        assert wake signal on RE9, wait 1 ms, deassert
* @param
* @return
*/
void Wake_signal()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //assert wake signal
    osDelay(2000); //spec says 3�s assertion, let's use ms delay and wait ~2 ms
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);  //de-assert wake signal
}
