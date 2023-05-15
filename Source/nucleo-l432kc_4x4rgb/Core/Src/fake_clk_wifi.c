/*
 * fake_clk_wifi.c
 *
 *  Created on: May 15, 2023
 *      Author: juro
 */


#include <time.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "fake_clk_wifi.h"
#include "assert.h"



static unsigned char fakePatternCodes[] = {
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 'z',
};


void fakeWifi_init()
{
	srand((unsigned int)time(NULL));
}


unsigned char fakeWifi_readCodePattern()
{
	int index = rand() % (sizeof(fakePatternCodes) / sizeof(fakePatternCodes[0]));
	return fakePatternCodes[index];
}


bool fakeWifi_send(const char *command, char *response, uint8_t response_size)
{
	if(strncmp(command, WIFIESP_IS_CONNECTED, sizeof(WIFIESP_IS_CONNECTED)) == 0)
	{
	    strncpy(response, "OK", response_size);
	    return true;
	}
	else
	{
		strncpy(response, "ERROR", response_size);
		return false;
	}
	return false;
}


