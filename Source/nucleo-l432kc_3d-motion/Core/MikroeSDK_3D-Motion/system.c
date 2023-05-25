#include "../MikroeSDK_3D-Motion/app.h"
#include "main.h"

/** Wake_signal
* @note        assert wake signal on RE9, wait 1 ms, deassert
* @param
* @return
*/
void Wake_signal()
{
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
	 HAL_Delay(2);
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);                                             //de-assert wake signal
}
