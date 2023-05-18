/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GA010
        Version           :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


/**
    Section: Includes
*/
#include <xc.h>
#include "pin_manager.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;
    LATE = 0x0200;
    LATF = 0x0000;
    LATG = 0x000C;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0xC680;
    TRISB = 0xFFFF;
    TRISC = 0xF01E;
    TRISD = 0xFFFF;
    TRISE = 0x01FF;
    TRISF = 0x11DF;
    TRISG = 0xF3C3;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPU1 = 0x0000;
    CNPU2 = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    AD1PCFG = 0x00C0;


    /****************************************************************************
     * Interrupt On Change for group CNEN1 - any
     ***************************************************************************/
	CNEN1bits.CN15IE = 1; // Pin : RD6

    /****************************************************************************
     * Interrupt On Change for group CNEN2 - any
     ***************************************************************************/
	CNEN2bits.CN19IE = 1; // Pin : RD13
	CNEN2bits.CN16IE = 1; // Pin : RD7

    IEC1bits.CNIE = 1; // Enable CNI interrupt 
}

// Addition for MM7150 application begin
typedef unsigned char UINT8;
typedef unsigned char BOOL;
#define TRUE 1
#define FALSE 0
#define _SW_S3_UP       0                                           //UP button
#define _SW_S6_DWN      1                                           //DOWN button
#define _SW_S5_SLCT     2                                           //SELECT button
#define _SW_S4_RST      3                                           //RESET (return to main menu) button

extern volatile UINT8 PB_EVT;                                       // Push Button press event
extern volatile BOOL DEBOUNCE;                                      // Debounce flag (initially set to FALSE, ie. NOT currently debouncing a switch closure)
// Addition for MM7150 application end

/* Interrupt service routine for the CNI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _CNInterrupt ( void )
{
    if(IFS1bits.CNIF == 1)
    {
        // Addition for MM7150 application begin
        if (!SW_S3_GetValue() && !DEBOUNCE)                         // (s3) button press caused interrupt & not currently debouncing switch closure
        {
            PB_EVT = _SW_S3_UP;                                     // set the press variable
            DEBOUNCE = TRUE;                                        // begin the debounce period
        }
    
        if (!SW_S6_GetValue() && !DEBOUNCE)                         // (s6) button press caused interrupt & not currently debouncing switch closure
        {
            PB_EVT = _SW_S6_DWN;                                    // set the press variable
            DEBOUNCE = TRUE;                                        // begin the debounce period
        }
    
        if (!SW_S4_GetValue() && !DEBOUNCE)                         // (s4) button press caused interrupt & not currently debouncing switch closure
        {
            PB_EVT = _SW_S4_RST;                                    // set the press variable
            DEBOUNCE = TRUE;                                        // begin the debounce period
        }
        // Addition for MM7150 application end
        // Clear the flag
        IFS1bits.CNIF = 0;
    }
}
