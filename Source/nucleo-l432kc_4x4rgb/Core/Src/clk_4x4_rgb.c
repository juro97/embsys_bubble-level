/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  Clock Frequency: 80MHz
 *  PWM Transmission Time for one 1 Bit: 1.25us
 * 	PWM Frequency: 1/1.25us = 800kHz
 * 	TIM1 Auto Reload Register: 800kHz/800kHz=100
 * 	TIM1 Capture Compare Register: 57/31
 *
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
 *	X = unknown error //TODO
 * 	Y = waiting for pairing //TODO
 * 	Z = reset, all led off //TODO
 *
 * 	"RED", "GREEN; "WHITE", etc.  =>  Identify LED to do an operation like setBrightness on
 *
 *
 */

/**************************************************************************
 ******************************** INCLUDES ********************************
 **************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"
#include "cmsis_os.h"

#include "clk_4x4_rgb.h"
#include "fake_clk_wifi.h"


/**************************************************************************
 ******************************** DEFINES *********************************
 **************************************************************************/

/* Comment in for the data to be received via the Wifi Test Module */
#define FAKE_WIFI


/* 16 LED * 24 Bit color values */
#define PWM_STREAM_LENGTH_FULL_DISPLAY		384

/* CCR Register Values for PWM Duty Cycle */
#define ONE 	57
#define ZERO	31


/**************************************************************************
 **************************** TYPEDEFS, VARS ******************************
 **************************************************************************/

/* Those are declared in the main.c file */
extern TIM_HandleTypeDef htim1;
extern osSemaphoreId_t sem_printPermissionHandle;


/* Datatype for one LED */
typedef struct __attribute__((__packed__))
{
	uint8_t green[8];
	uint8_t red[8];
	uint8_t blue[8];
} rgb_led;


/* Datatype for full 4x4 Matrix */
typedef struct  __attribute__((__packed__))
{
	rgb_led led[16];
} rgb_pattern;



/* Predefined Colors for one LED */
const rgb_led off =
{
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
};

const rgb_led all_on =
{
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
};

const rgb_led red =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ONE, ONE, ZERO, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led green =
{
	{ZERO, ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led blue =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ONE, ONE, ZERO, ONE, ONE},
};


const rgb_led yellow =
{
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led orange =
{
	{ZERO, ZERO, ONE, ZERO, ZERO, ONE, ZERO, ONE},
	{ZERO, ZERO, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},

};

const rgb_led white =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
};



/* Predefined Patterns for the Matrix */

const rgb_pattern rgb4x4click_centered =
{{
	white, 	white, 	white, 	white,
	white, 	green, 	green, 	white,
	white, 	green, 	green, 	white,
	white, 	white, 	white,	white
}};

const rgb_pattern rgb4x4click_outOfScope =
{{
	red, 	red, 	red, 	red,
	red, 	white,	white,	red,
	red, 	white,	white,	red,
	red, 	red, 	red, 	red,
}};

const rgb_pattern rgb4x4click_shakeDetection =
{{
	blue,	off, 	off, 	blue,
	off, 	blue, 	blue, 	off,
	off, 	blue, 	blue, 	off,
	blue, 	off, 	off, 	blue
}};

const rgb_pattern rgb4x4click_lightShift_lu =
{{
	yellow, yellow, yellow, off,
	yellow, green, 	yellow, off,
	yellow, yellow, yellow, off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_lightShift_lo =
{{
	off, 	off, 	off, 	off,
	yellow, yellow, yellow, off,
	yellow, green, 	yellow, off,
	yellow, yellow, yellow,	off
}};

const rgb_pattern rgb4x4click_lightShift_ru =
{{
	off, 	yellow, yellow, yellow,
	off, 	yellow, green, 	yellow,
	off, 	yellow, yellow, yellow,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_lightShift_ro =
{{
	off, 	off, 	off, 	off,
	off, 	yellow, yellow, yellow,
	off, 	yellow, green, 	yellow,
	off, 	yellow, yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r1c1 =
{{
	red,	yellow,	off,	off,
	yellow,	yellow,	off,	off,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c2 =
{{
	yellow,	red, 	yellow, off,
	yellow,	yellow,	yellow,	off,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c3 =
{{
	off,	yellow,	red,	yellow,
	off,	yellow,	yellow,	yellow,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c4 =
{{
	off, 	off, 	yellow,	red,
	off, 	off,	yellow,	yellow,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c1 =
{{
	yellow,	yellow,	off,	off,
	red,	yellow,	off,	off,
	yellow,	yellow,	off,	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c4 =
{{
	off, 	off, 	yellow, yellow,
	off,	off, 	yellow, red,
	off, 	off, 	yellow, yellow,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r3c1 =
{{
	off,	off, 	off, 	off,
	yellow, yellow, off, 	off,
	red, 	yellow, off, 	off,
	yellow, yellow, off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r3c4 =
{{
	off,	off,	off,	off,
	off,	off, 	yellow,	yellow,
	off,	off,	yellow, red,
	off,	off,	yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c1 =
{{
	off,	off, 	off, 	off,
	off, 	off, 	off, 	off,
	yellow, yellow, off, 	off,
	red, 	yellow, off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c2 =
{{
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
	yellow, yellow, yellow, off,
	yellow, red, 	yellow, off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c3 =
{{
	off,	off,	off,	off,
	off,	off, 	off, 	off,
	off,	yellow, yellow, yellow,
	off,	yellow, red,  	yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c4 =
{{
	off,	off, 	off,	off,
	off,	off, 	off, 	off,
	off,	off, 	yellow, yellow,
	off, 	off,	yellow, red
}};

const rgb_pattern rgb4x4click_error =
{{
	off,	off, 	off,	off,
	off,	off, 	off, 	off,
	off,	off, 	yellow, yellow,
	off, 	off,	yellow, red
}};

const rgb_pattern rgb4x4click_pairing =
{{
	off,	off, 	off,	off,
	off,	off, 	off, 	off,
	off,	off, 	yellow, yellow,
	off, 	off,	yellow, red
}};

const rgb_pattern rgb4x4click_all_off =
{{
	off, 	off, 	off, 	off,
	off, 	off,	off, 	off,
	off, 	off, 	off, 	off,
}};


/* Pointer to the RGB Matrix Patterns */
rgb_pattern const *ptr_Rgb4x4Click = NULL;




/**************************************************************************
 ******************************** METHODS *********************************
 **************************************************************************/

/**
 * @brief	Prints the received Data from the Broker and prints it on the 4x4 Matrix
 * @param	ch - Pointer to a Char holding the Information to print
 */
void printDataOnMatrix(char const *ch)
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


//TODO
/**
 * @brief	Calls the Broker for new Sensor Data
 * @param	pData - Pointer to Data Buffer
 * @param	pSize - Pointer to Variable holding the Size of the Buffer
 * @return	false if Buffer to Small, Broker not reachable, inreliable sensor values, else true
 */
bool getSensorDataFromServer(const *pData, const size_t const *pSize)
{
	// check if buffer size is enough, else return false
	// check if server is reachable, else return false
	// check if server values are reliable, else return false
	// write new data into buffer
	// return true
}


//TODO
/**
 * @brief	Checks if the Broker is eachable
 * @return	false if not reachable
 */
bool serverReachable()
{

}


//TODO
/**
 * @brief	Sets the Brightness of one single Color
 * @param	colCode - The Color Code to identify the LED Color like "GREEN" or "RED"
 * @param	brightness - Value between 1 and 10 for Intensity
 * @return	false when there were wrong input parameters, else true
 */
bool setSingleColorBrightness(const char const* colCode, const uint8_t const *brightness)
{

	/*
	 * Check Input Parameters
	 * Map colCode and brightness to according categories
	 */
}


//TODO
/**
 * @brief	Sets the Brightness of all Colors
 * @param	brightness - Value between 1 and 10 for Intensity
 * @return	false when there were wrong input parameters, else true
 */
bool setAllColorBrightnesses(const uint8_t const *brightness)
{
	/*
	 * Check Input Parameters
	 * Map brightness to according categories
	 */
}

