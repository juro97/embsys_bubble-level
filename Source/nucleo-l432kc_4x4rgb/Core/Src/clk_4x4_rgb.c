/*
 *  Taktfrequenz: 80MHz
 *  Uebertragungsdauer 1 Bit: 1.25uS
 * 	Frequenz PWM: 1/1.25uS = 800kHz
 * 	Auto Reload Register: 800kHz/800kHz=100
 * 	Capture Compare Register: 33/66
 */

/* Codes
 * ==================
 * a = middle
 * b = out of scope
 * c = shake detected
 * d = light shift lo
 * e = light shift lu
 * f = light shift ro
 * g = light shift ru
 * h = strong shift r1c1
 * i = strong shift r1c2
 * j = strong shift r1c3
 * k = strong shift r1c4
 * l = strong shift r2c1
 * m = strong shift r2c4
 * n = strong shift r3c1
 * o = strong shift r3c4
 * p = strong shift r4c1
 * q = strong shift r4c2
 * r = strong shift r4c3
 * s = strong shift r4c4
 * z = off
 */

/*************************************
 *********** INCLUDES ****************
 *************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main.h"
#include "cmsis_os.h"

#include "clk_4x4_rgb.h"
#include "fake_clk_wifi.h"


/*************************************
 *********** DEFINES *****************
 *************************************/

/* 1/4/16 LED * 24 Bit color values */
#define PWM_STREAM_LENGTH_1_LED 			24
#define PWM_STREAM_LENGTH_1_ROW			 	96
#define PWM_STREAM_LENGTH_FULL_DISPLAY		384

/* Duty Cycle wurde geschätzt, da unklar in DB */
#define NONE 	0
#define ONE 	57
#define ZERO	31


/****************************************************
 *********** Variables, Typedefs, .. ****************
 ****************************************************/

/* those are declared in the main.c file */
extern TIM_HandleTypeDef htim1;
extern osSemaphoreId_t sem_printPermissionHandle;

//GyroValues: Datentyp noch anpassen;; Daten können per DMA geändert werden, daher volatile
volatile char patternCode = 'z';


/* datatypes for the colors and patterns */
typedef struct __attribute__((__packed__)) {
	uint8_t green[8];  	/*8 bits of the color represented by logic one (60) and by logic zero (40) */
	uint8_t red[8];
	uint8_t blue[8];
} rgb_led;

typedef struct  __attribute__((__packed__)) {
	rgb_led led[16];
} rgb_pattern;


/* predefined colors */
/* 0x000000 */
const rgb_led off = {
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
};

/* 0xFFFFFF */
const rgb_led all_on = {
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
};

/* 0xFF0000 */
const rgb_led red = {
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ONE, ONE, ZERO, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

/* 0x00FF00 */
const rgb_led green = {
	{ZERO, ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

/* 0x0000FF */
const rgb_led blue = {
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ONE, ONE, ZERO, ONE, ONE},
};

/* 0xFFFF00 */
const rgb_led yellow = {
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

/* 0xFFA500 => b */
const rgb_led orange = {
	{ZERO, ZERO, ONE, ZERO, ZERO, ONE, ZERO, ONE},
	{ZERO, ZERO, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},

};

/* 0x646464 => b */
const rgb_led light_white = {
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
};

/* 0xFFFFFF*/
const rgb_led strong_white = {
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
};



/* predefined patterns */

const rgb_pattern rgb4x4click_all_off = {{
		off, off, off, off,
		off, off, off, off,
		off, off, off, off,
}};

const rgb_pattern rgb4x4click_all_on = {{
		all_on, all_on, all_on, all_on,
		all_on, all_on, all_on, all_on,
		all_on, all_on, all_on, all_on,
}};

const rgb_pattern rgb4x4click_centered = {{
	light_white, light_white, 	light_white, 	light_white,
	light_white, green, 		green, 			light_white,
	light_white, green, 		green, 			light_white,
	light_white, light_white, 	light_white,	light_white
}};

const rgb_pattern rgb4x4click_outOfScope = {{
	red, red, red, red,
	red, light_white, light_white, red,
	red, light_white, light_white, red,
	red, red, red, red,
}};

const rgb_pattern rgb4x4click_shakeDetection = {{
	blue,	off, 	off, 	blue,
	off, 	blue, 	blue, 	off,
	off, 	blue, 	blue, 	off,
	blue, 	off, 	off, 	blue
}};

const rgb_pattern rgb4x4click_lightShift_lu = {{
	yellow, yellow, yellow, off,
	yellow, green, 	yellow, off,
	yellow, yellow, yellow, off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_lightShift_lo = {{
	off, off, off, off,
	yellow, yellow, yellow, off,
	yellow, green, yellow, off,
	yellow, yellow, yellow,off
}};

const rgb_pattern rgb4x4click_lightShift_ru = {{
	off, yellow, yellow, yellow,
	off, yellow, green, yellow,
	off, yellow, yellow, yellow,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_lightShift_ro = {{
	off, off, off, off,
	off, yellow, yellow, yellow,
	off, yellow, green, yellow,
	off, yellow, yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r1c1 = {{
	red,yellow,off,off,
	yellow,yellow,off,off,
	off, off, off, off,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c2 = {{
	yellow, red, yellow, off,
	yellow,yellow,yellow,off,
	off, off, off, off,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c3 = {{
	off,yellow,red,yellow,
	off,yellow,yellow,yellow,
	off, off, off, off,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c4 = {{
	off, off, yellow,red,
	off, off,yellow,yellow,
	off, off, off, off,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c1 = {{
	yellow,yellow,off,off,
	red,yellow,off,off,
	yellow,yellow,off,off,
	off, off, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c4 = {{
	off, 	off, 	yellow, 	yellow,
	off,	off, 	yellow, 	red,
	off, 	off, 	yellow, 	yellow,
	off, 	off, 	off, 		off,
}};

const rgb_pattern rgb4x4click_strongShift_r3c1 = {{
	off,	off, 	off, off,
	yellow, yellow, off, off,
	red, 	yellow, off, off,
	yellow, yellow, off, off
}};

const rgb_pattern rgb4x4click_strongShift_r3c4 = {{
	off,	off,	off,	off,
	off,	off, 	yellow,	yellow,
	off,	off,	yellow, red,
	off,	off,	yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c1 = {{
	off,	off, 	off, off,
	off, 	off, 	off, off,
	yellow, yellow, off, off,
	red, 	yellow, off, off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c2 = {{
	off, 	off, 	off, 		off,
	off, 	off, 	off, 		off,
	yellow, yellow, yellow, 	off,
	yellow, red, 	yellow, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c3 = {{
	off,	off,	off,		off,
	off,	off, 	off, 		off,
	off,	yellow, yellow, 	yellow,
	off,	yellow, red,  		yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c4 = {{
	off,	off, 	off,	 	off,
	off,	off, 	off, 		off,
	off,	off, 	yellow, 	yellow,
	off, 	off,	yellow, 	red
}};

/* For testing purpose only */
const rgb_pattern rgb4x4click_onlyOneLed = {{
		red, off, off, off,
		red, off, off, off,
		red, off, off, off,
		red, off, off, off,
}};

const rgb_pattern rgb4x4click_twoLed = {{ red, green }};
const rgb_pattern rgb4x4click_oneRow = {{ red, green, blue, strong_white }};


/* pointer to a rgb_pattern for switching */
rgb_pattern const *ptr_Rgb4x4Click = NULL;



/*************************************
 ***** METHOD Implementations*********
 *************************************/

/**
 * takes a pointer to a character as argument and prints the pattern
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
		case 'z':
			ptr_Rgb4x4Click = &rgb4x4click_strongShift_r1c1;
			break;
		case '1':
			ptr_Rgb4x4Click = &rgb4x4click_all_off;
			break;
		case '4':
			ptr_Rgb4x4Click = &rgb4x4click_oneRow;
			break;
		case 'R':
			ptr_Rgb4x4Click = &rgb4x4click_all_off;
			break;
		case 'N':
			ptr_Rgb4x4Click = &rgb4x4click_all_on;
			break;
		default:
			/* You should not be here, might code error handling later */
			ptr_Rgb4x4Click = &rgb4x4click_shakeDetection;
			break;

	}


	osSemaphoreAcquire(sem_printPermissionHandle, osWaitForever);

	/* actually not really necessary and quite useless, just to be 101% sure*/
	taskENTER_CRITICAL();

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *) ptr_Rgb4x4Click, PWM_STREAM_LENGTH_FULL_DISPLAY);

	taskEXIT_CRITICAL();

}


bool serverReachable()
{
	/* FAKE wifi module */
	static char fakeReceiveBuffer[64];
	static bool isReachable = false;

	/* we dont do anything with te fakeReceivedData when pinging, yet */
	isReachable = fakeWifi_send(WIFIESP_IS_CONNECTED, fakeReceiveBuffer, sizeof(fakeReceiveBuffer));

	return isReachable;
	/* real wifi module */
	/* return wiFiClick_SendATCommand("AT-CWSTATE"); */
}


/* Diese Funktion wird Periodisch 5x pro Sekunde aus dem Task per Software Timer aufgerufen */

/* Hole die Daten vom Server */
bool getSensorDataFromServer()
{
	if( serverReachable() == true )
	{
		//printDataOnMatrix();

		return true;
	}

	//printDataOnMatrix('z');

	return false;
}

