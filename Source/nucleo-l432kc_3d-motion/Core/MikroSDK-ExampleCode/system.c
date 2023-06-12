#include "app.h"

#include "main.h"

/** Wake_signal
* @brief: assert wake signal on PA0, wait 1 ms, deassert
*/
void Wake_signal()
{
	/* assert wake signal */
    HAL_GPIO_WritePin(MOTION_WAKE_PIN_GPIO_Port, MOTION_WAKE_PIN_Pin, GPIO_PIN_RESET);

    /* spec says 3�s assertion, let's use ms delay and wait ~2 ms */
    HAL_Delay(10);

    /* de asser wake signal */
    HAL_GPIO_WritePin(MOTION_WAKE_PIN_GPIO_Port, MOTION_WAKE_PIN_Pin, GPIO_PIN_SET);
}


/** Reset_signal
 * @brief: assert wake signal on PB1, wait 1 ms, deassert
 */
void Reset_signal()
{
	/* assert reset signal */
	HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_RESET);

	/* spec says 3�s assertion, let's use ms delay and wait ~2 ms */
	HAL_Delay(10);

	/* de asser reset signal */
	HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_SET);
}
