/*
 * clk_4x4_rgb.h
 *
 *  Created on: May 14, 2023
 *      Author: juro
 */

#ifndef INC_CLK_4X4_RGB_H_
#define INC_CLK_4X4_RGB_H_


#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

bool serverReachable(void);
bool connectToServer(void);
void printDataOnMatrix(const char * const ch);
bool getMotionDataFromServer(const char *pData);
bool setSingleColorBrightness(const char * const colCode, const uint8_t * const brightness);
bool setAllColorBrightnesses(const uint8_t * const brightness);


#endif /* INC_CLK_4X4_RGB_H_ */
