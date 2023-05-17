/*
 * clk_3dmotion.c
 *
 *  Created on: May 17, 2023
 *      Author: juro
 */

#include "main.h"

#include "clk_3dmotion.h"


/* does all the needed initialization stuff */
void init_mm7150()
{

	// read the hid descriptors
	// parse the hid descriptors to get to the information
	// further initialization

}



void i2c_read_data_from_mm7150()
{
	//request data report from module 5x per second
		// dataAvailable must be true
		// best case over dma; or i2c_it
		//best case via dma
	//parse the received data
}

/* convert the sensor data into descrete values*/
void process_data_from_mm7150()
{
	//further process the data
	//no comma values
}


/* pseudo code */
ISR_dataAvailable()
{
	dataAvailable = true;
}


