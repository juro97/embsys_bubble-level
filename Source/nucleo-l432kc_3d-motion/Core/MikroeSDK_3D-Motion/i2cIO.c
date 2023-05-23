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
/** @file  i2cIO.c
*   I2C functions to I/F PIC24 with SSC7150
*****************************************************************************
*   SSC7150 Sensor Fusion with Explorer 16 Development Board Demo and Sample
*       Code i2c i/o file
*
*   Company : Microchip Technology Inc.
*
*   File name : i2cIO.c
*
*   Summary : Module for SSC7150 Sensor Fusion demo code which interfaces
*           PIC24 (Explorer16 board) to SSC7150 via i2c bus/calls
*
*   Functions : i2c_cmd_WrRd
*               gets_I2C1
*               i2cIO_error
*
*   Revisions : 0.2 9-18-14 C21674 - enhanced error handling, added timer2 interrupt
*                                    to exit for unresponsive i2c, gets_I2C1 check status
*                                    bits on exit with no data
*             : 0.1 8-4-14 C21674
*             : 0.0 7-1-14 C16368  - Initial version created
******************************************************************************/
//****************************************************************************
//****************************************************************************
//  Section : Included files
//****************************************************************************
//****************************************************************************

#include "../MikroeSDK_3D-Motion/app.h"

volatile uint16_t status = 0;

void error_handler(char *str, UINT16 offset, UINT16 errval) {
  char err_buf[64];
  UINT16 wait_cnt;

  //output to uart
  sprintf(err_buf, "***Error in function: %s:%02X errval=0x%X*** \r\n", str, offset, errval);
  //TFT_Set_Font(TFT_defaultFont, CL_BLACK, FO_HORIZONTAL);
  //TFT_Write_Text(err_buf, 10, 10);
  
  StopI2CTimer();
  I2C_Stop();
}

volatile BOOL TIMER_1MS_FLG = FALSE;                                        // 1 msec timer interrupt flag

volatile BOOL EC_DATA_AVAIL = FALSE;                                // EC data available for read flag

//*****************************************************************************
//*****************************************************************************
//  Section : Code
//*****************************************************************************
//*****************************************************************************

/** gets_I2C1
* @note        Read bytes from sensor device via i2c1 channel slave
* @param ucRdptr pointer to data buffer from slave
* @param usLength number of bytes to read from slave
* @param bAdjust  first bytes of rx packet has number of bytes returned by read, if =TRUE use this new data length
* @return
*/
void gets_I2C1(UINT8 *ucRdptr, UINT16 usLength, BOOL bAdjust) {

  UINT16 i = 0;
  UINT8 ucSize = 1;                                                // Set return value for size of data read for bAdjust=FALSE
  UINT16 usStat = 0;


  while (usLength--)
  {
    if (usLength)                                               // bytes to be read
    {
      ucRdptr[i] = I2C_Read(_I2C_ACK);
      i++;
    }
    else {
      ucRdptr[i++] = I2C_Read(_I2C_NACK);
    }

    if (bAdjust && i == 2)                                      // Read first 2 bytes which have the length of the packet
    {
      usLength = ((ucRdptr[1] << BYTE_SHIFT) | ucRdptr[0]) - 2; // Actual length is value of first 2 bytes minus 2 (because we read 2 already)
      ucSize = usLength;
    }
  }

  if (!ucSize)
  {
    error_handler("i2c ", 0, I2C_ERROR);                     //flag as general i2c error (does not return)
  }

}


/** i2c_cmd_WrRd
* @note        i2c write,read, and combined write/read commands, start timer2 interrupt to exit on unresponsive i2c bus
* @param ucCmd error code
* @param ucBytes_wr Number of bytes to write to slave
* @param ucData_wr Pointer to data buffer to send to slave
* @param usBytes_rd Number of bytes to read from slave
* @param ucData_rd Pointer to data buffer from slave
* @param bAdjust Use 1st 2 bytes returned as new length (=TRUE)
* @return I2C_SUCCESS(=0), I2C_BUF_OVRFLO(=0x22)
*/
UINT8 i2c_cmd_WrRd(UINT8 ucCmd, UINT8 ucBytes_wr, UINT8 *ucData_wr, UINT16 usBytes_rd, UINT8 *ucData_rd, BOOL bAdjust)
{
  UINT8 i;


  if (ucBytes_wr > BUF_150)                                       // sanity check for maximum buffer size
    return I2C_BUF_OVRFLO;                                      // return i2c buffer overflow error code to calling routine

  StartI2CTimer();                                                // start timer2 interrupt in case i2c hangs in 'while loop' functions

  status = I2C_Start();
  if (status)
    error_handler("i2c ", 0, I2C_ERROR);

  switch (ucCmd)
  {
  case WRITE:
    status = I2C_Write(SLAVE_ADDR);
    if (status)
      error_handler("i2c ", 0, I2C_ERROR);
    for (i = 0; i < ucBytes_wr; i++)                     // Begin a loop writing the tx bytes to the slave
    {
      status = I2C_Write(ucData_wr[i]);
      if (status)
        error_handler("i2c ", 0, I2C_ERROR);
    }

    break;

  case READ:
    status = I2C_Write(SLAVE_ADDR | 1);
    if (status)
      error_handler("i2c ", 0, I2C_ERROR);

    gets_I2C1(ucData_rd, usBytes_rd, bAdjust);          // Read in multiple bytes

    break;

  case WR_RD:
    status = I2C_Write(SLAVE_ADDR);
    if (status)
      error_handler("i2c ", 0, I2C_ERROR);

    for (i = 0; i < ucBytes_wr; i++)                         // Begin a loop writing the tx bytes to the slave
    {
      status = I2C_Write(ucData_wr[i]);
      if (status)
        error_handler("i2c ", 0, I2C_ERROR);
    }

    status = I2C_Restart();
    if (status)
      error_handler("i2c ", 0, I2C_ERROR);

    status = I2C_Write(SLAVE_ADDR | 1);
    if (status)
      error_handler("i2c ", 0, I2C_ERROR);

    gets_I2C1(ucData_rd, usBytes_rd, bAdjust);              // Read in multiple bytes

    break;
  }

  I2C_Stop();                                                     // Send STOP condition

  StopI2CTimer();                                                 //turn off timer2 interrupt

  return I2C_SUCCESS;
}