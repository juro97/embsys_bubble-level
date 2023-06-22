/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *																   *
 *  Clock Frequency: 80MHz										   *
 *  PWM Transmission Time for one 1 Bit: 1.25us					   *
 * 	PWM Frequency: 1/1.25us = 800kHz							   *
 * 	TIM1 Auto Reload Register: 800kHz/800kHz=100				   *
 * 	TIM1 Capture Compare Register: 57/31						   *
 *																   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * 	Received Data Codes
 * 	==================
 * 	a = middle
 * 	b = out of scope
 * 	c = shake detected
 * 	d = light shift lo
 * 	e = light shift lu
 * 	f = light shift ro
 * 	g = light shift ru
 * 	h = strong shift r1c1
 * 	i = strong shift r1c2
 * 	j = strong shift r1c3
 * 	k = strong shift r1c4
 * 	l = strong shift r2c1
 * 	m = strong shift r2c4
 * 	n = strong shift r3c1
 * 	o = strong shift r3c4
 * 	p = strong shift r4c1
 * 	q = strong shift r4c2
 * 	r = strong shift r4c3
 * 	s = strong shift r4c4
 *
 *	X = unknown error
 *
 *
 */

/**************************************************************************
 ******************************** INCLUDES ********************************
 **************************************************************************/

#include "main.h"

#include "patterns.h"


/**************************************************************************
 ******************************** DEFINES *********************************
 **************************************************************************/

/* Comment in for the data to be received via the Wifi Test Module */
#define VIRTUAL_WIFI

/* 16 LED * 24 Bit color values */
#define PWM_STREAM_LENGTH_FULL_DISPLAY		384


/**************************************************************************
 **************************** TYPEDEFS, VARS ******************************
 **************************************************************************/

/* Those are declared in the main.c file */
extern TIM_HandleTypeDef htim1;
extern osSemaphoreId_t sem_printPermissionHandle;

/* Pointer to the RGB Matrix Patterns */
rgb_pattern const *ptr_Rgb4x4Click = NULL;


/**************************************************************************
 ******************************** METHODS *********************************
 **************************************************************************/

/**
 * @brief	Prints the received Data from the Broker and prints it on the 4x4 Matrix
 * @param	ch - Pointer to a Char holding the Information to print
 */
void printDataOnMatrix(const char * const ch)
{
	switch(*ch)
	{
		case 'a':
			ptr_Rgb4x4Click = &rgb4x4click_centered;
			break;
		case 'b':
			ptr_Rgb4x4Click = &rgb4x4click_outOfScope;
			break;
		case 'c':
			ptr_Rgb4x4Click = &rgb4x4click_shakeDetection;
			break;
		case 'd':
			ptr_Rgb4x4Click = &rgb4x4click_lightShift_lo;
			break;
		case 'e':
			ptr_Rgb4x4Click = &rgb4x4click_lightShift_lu;
			break;
		case 'f':
			ptr_Rgb4x4Click = &rgb4x4click_lightShift_ro;
			break;
		case 'g':
			ptr_Rgb4x4Click = &rgb4x4click_lightShift_ru;
			break;
		case 'h':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r1c1;
			break;
		case 'i':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r1c2;
			break;
		case 'j':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r1c3;
			break;
		case 'k':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r1c4;
			break;
		case 'l':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r2c1;
			break;
		case 'm':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r2c4;
			break;
		case 'n':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r3c1;
			break;
		case 'o':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r3c4;
			break;
		case 'p':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r4c1;
			break;
		case 'q':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r4c2;
			break;
		case 'r':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r4c3;
			break;
		case 's':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r4c4;
			break;
		case 'X':
			ptr_Rgb4x4Click = &rgb4x4click_error;
		case 'Y':
			ptr_Rgb4x4Click = &rgb4x4click_pairing;
			break;
		case 'Z':
			ptr_Rgb4x4Click = &rgb4x4click_all_off;
			break;
		default:
			/* You should not be here */
			ptr_Rgb4x4Click = &rgb4x4click_error;
			break;
	}

	/* Wait until previous DMA Transfer is Completed */
	osSemaphoreAcquire(sem_printPermissionHandle, osWaitForever);

	/* Actually not really necessary and quite useless, just to be 101% sure*/
	taskENTER_CRITICAL();

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *) ptr_Rgb4x4Click, PWM_STREAM_LENGTH_FULL_DISPLAY);

	taskEXIT_CRITICAL();

}


