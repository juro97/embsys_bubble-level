/** @file  app.h
*   Generic include header file
***************************************************************************
*   Summary :  Top level include header file for SSC7150 Sensor Fusion demo 
*               code 
******************************************************************************/
#ifndef APP_H
#define        APP_H

#ifdef        __cplusplus
extern "C" {
#endif

//****************************************************************************
//  Section : Included files
//****************************************************************************

#define __GENERIC_TYPE_DEFS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

typedef unsigned char UINT8;        // 8-bit variables
typedef unsigned char BOOL;
   
typedef unsigned int UINT16;      // 16-bit variables
typedef unsigned long UINT32;
#define TRUE 1
#define FALSE 0

//  Peripheral Include Files
#include "System.h"
#include "interrupts.h"
#include "vregs.h"
#include "i2cIO.h"
#include "sf.h"
#include "flashupdate.h"
#include "err.h"
#include "sensors.h"

//****************************************************************************
//  Section : Defines
//****************************************************************************

//menu selection 

#define MENU_ITEMS          14

#define MENU_STR_LEN        19
#define MIN_MENU_SLCT            2


#define MAX_MENU_SLCT            12       
#define MAX_LINE_NUM            13

#define ACCEL_MENU_SLCT     2       //accelerometer
#define GYRO_MENU_SLCT      3       //gyrometer
#define COMP_MENU_SLCT      4       //compass
#define ORI_MENU_SLCT       5       //orientation
#define INCL_MENU_SLCT      6       //inclinometer
#define RAW_ACC_MENU_SLCT   7       //raw accelerometer
#define RAW_MAG_MENU_SLCT   8       //raw magnometer
#define RAW_GYRO_MENU_SLCT  9       //raw gyrometer
#define SLP_MENU_SLCT                10                //sleep 
#define WAKE_MENU_SLCT                11                //wake
#define FLASH_UPDATE_MENU_SLCT  12  //flash update



#define DBNCE_VAL           300     // Debounce timer value (empirically derived)
#define RAW_DATA_RATE       100
#define RAW_SENSITIVITY     100
#define RAW_SENSITIVITY_MIN 0x0


#ifdef        __cplusplus
}
#endif

#endif        /* APP_H */
