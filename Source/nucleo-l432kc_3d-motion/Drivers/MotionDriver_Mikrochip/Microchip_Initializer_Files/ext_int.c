/**
  EXT_INT Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    ext_int.c

  @Summary
    This is the generated driver implementation file for the EXT_INT 
    driver using MPLAB(c) Code Configurator

  @Description:
    This source file provides implementations for driver APIs for EXT_INT. 
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC24FJ128GA010
        Driver Version    :  1.0
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.26
        MPLAB             :  MPLAB X 3.20
*/

/**
   Section: Includes
 */
#include <xc.h>
#include "ext_int.h"
//***User Area Begin->code: Add External Interrupt handler specific headers 
#include <stdint.h>
#include <stdbool.h>
typedef unsigned char BOOL;
extern volatile BOOL EC_DATA_AVAIL;                          // EC data available for read flag
extern volatile BOOL bFF_INT;                                // EC data available for read flag
//***User Area End->code: Add External Interrupt handler specific headers

/**
   Section: External Interrupt Handlers
 */
/**
  Interrupt Handler for EX_INT1 - INT1
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _INT1Interrupt(void)
{
    //***User Area Begin->code: INT1 - External Interrupt 1***
    if (!EC_DATA_AVAIL)                                         // If a falling edge occurred (data is available from EC)
    {
        EX_INT1_PositiveEdgeSet();
        INTCON2bits.INT1EP = 0;                                 // INT1 Edge configured to interrupt on rising edge (wait for end of data)
        EC_DATA_AVAIL = true;                                   // Toggle EC_DATA_AVAIL flag to notify data received
        bFF_INT = true;                                         //additional flag for free-fall demo (cleared in main.c function))
    }
    else
    {
        EX_INT1_NegativeEdgeSet();
        EC_DATA_AVAIL = false;                                  // interrupt de-asserted
    }
    //***User Area End->code: INT1 - External Interrupt 1***
    EX_INT1_InterruptFlagClear();
}
/**
    Section: External Interrupt Initializers
 */
/**
    void EXT_INT_Initialize(void)

    Initializer for the following external interrupts
    INT1
*/
void EXT_INT_Initialize(void)
{
    /*******
     * INT1
     * Clear the interrupt flag
     * Set the external interrupt edge detect
     * Enable the interrupt, if enabled in the UI. 
     ********/
    EX_INT1_InterruptFlagClear();   
    EX_INT1_NegativeEdgeSet();
    EX_INT1_InterruptEnable();
}
