#include "main.h"

/** Wake_signal
* @note        Assert Wake and Reset Signal on Specified Portpin with Delay According to Datasheet
* @param
* @return
*/
void Wake_signal()
{
	/* Assert Wake Signal */
	 HAL_GPIO_WritePin(MOTION_WAKE_PIN_GPIO_Port, MOTION_WAKE_PIN_Pin, GPIO_PIN_RESET);
	/* Assert Wake Signal */
	//HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_SET);

	 /* TODO: (1/2) Test different Timing Variants because the 2ms according to the Reference Manual does not work! */
	/* Wait 2ms */
    HAL_Delay(20);

    /* De-Assert Wake Signal */
    HAL_GPIO_WritePin(MOTION_WAKE_PIN_GPIO_Port, MOTION_WAKE_PIN_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_RESET);
}

void Reset_signal()
{
	/* Assert Wake Signal */
	HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_RESET);

	/* TODO: (2/2) Test different Timing Variants because the 11ms according to the Reference Manual does not work! */
	/* Wait 11ms */
	HAL_Delay(20);

	/* De-Assert Wake Signal */
	HAL_GPIO_WritePin(MOTION_RESET_PIN_GPIO_Port, MOTION_RESET_PIN_Pin, GPIO_PIN_SET);
}
