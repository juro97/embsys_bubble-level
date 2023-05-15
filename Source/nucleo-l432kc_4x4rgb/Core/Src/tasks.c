/*
 * tasks.c
 *
 *  Created on: May 14, 2023
 *      Author: juro
 */

#include "tasks.h"
#include "clk_4x4_rgb.h"


/* Task 0 ConnectToServer */
//Highes Priority
//Executed first when module startup
//When connected => Terminated or blocked
//then call task 1




/* Task 1 */
//Periodically Retrieve New Values
//Normal Priority
//if connection lost over some time ==> Retrieve Task 1
//Yield Between the Periods so that Task 2 can be fired





/* Task 2 */
//Recheck the Incoming Data From the Matrix
//Lowest Priority
//Only Reevaluate the data if there is cpu time available
//if there is a mismatch, show if via the led on the extension board
// every other task interrupts this one ASAP
//when reentering this task => start new reading, not the old one

