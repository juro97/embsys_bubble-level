/*****************************************************************************
* � 2014 Microchip Technology Inc. and its subsidiaries.
* You may use this software and any derivatives exclusively with
* Microchip products.
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".
* NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
* AND FITNESS FOR A PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
* TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
* CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF
* FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*****************************************************************************/
/** @file  system.h
*   Header file for Explorer16 + PIC24 system level functions
*****************************************************************************
*   SSC7150 Sensor Fusion with Explorer 16 Development Board Demo and Sample
*           Code header file
*
*   Company : Microchip Technology Inc.
*
*   File name : System.h
*
*   Summary :  Include file for SSC7150 Sensor Fusion demo code 
*
*   Revisions : 0.3 C21674 - added DEBUG_MODE for toggling signal
*             : 0.2 9-30-14 C21674 - timer & uart global params added
*             : 0.1 8-4-14 C21674  - 
*             : 0.0 7-1-14 C16368  - Initial version created
******************************************************************************/


#ifndef SYSTEM_H
#define        SYSTEM_H

#ifdef        __cplusplus
extern "C" {
#endif

//****************************************************************************
//****************************************************************************
//  Section : Defines
//****************************************************************************
//****************************************************************************


//****************************************************************************
//****************************************************************************
//  Section : Function Declarations
//****************************************************************************
//****************************************************************************

void Wake_signal(void);
void Reset_signal(void);

#ifdef        __cplusplus
}
#endif

#endif        /* SYSTEM_H */
