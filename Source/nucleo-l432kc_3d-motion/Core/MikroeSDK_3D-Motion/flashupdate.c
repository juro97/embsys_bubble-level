/** @file  flashupdate.c
*   Flash Update functions 
*****************************************************************************
*   Summary : Module for SSC7150 Sensor Fusion demo code which implements 
*           Flash Update process 
*
*   Functions : flash_info_command
*               flash_write_command
*               flash_verify_command
******************************************************************************/

//****************************************************************************
//  Section : Included files
//****************************************************************************
          
#include "app.h"

//*****************************************************************************
//  Section : File scope variables and functions
//*****************************************************************************

extern HID_DESCRIPTOR_TABLE HID_FIELD;                              //structure for HID Descriptor


UINT16 seq_number = 0;
UINT32 image_crc = 0;
BOOL img_wr_done = FALSE;


char ErrorString[][50] =                                            //List of errors returned by SSC7150 firmware
{
        "SPI Communication failure",
        "Flash Protect Error",
        "Flash Write Error",
        "Flash Read Error",
        "Decrypt Signature Error",
        "Packet Framing Error",
        "Packet Checksum Error",
        "Header verification Error",
        "Verification Failure",
        "Command Timeout",
        "Wrong SP Parmeters",
        "Wrong Flash parameters",
        "TAG Write Error",
        "Invalid TAGs \nPlease verify the input file",
        "Image Length Error",
        "Invalid CRC Error",
        "Invalid Flash Address",
        "Invalid Data Length",
        "Unknown Error, Error code out of range"
};


//*****************************************************************************
//  Section : Code
//*****************************************************************************

/** flash_info_command
* @note        Implements flash info(+ reset) command during flash update
* @param ucCmdDatbuf pointer to data to be sent as part of flash info command
* @param cmd command byte for reset or bootloader_info sub-command
* @return completion_status 0=SUCCESS, failcode: 0x17=SET_FEAT_FAIL,0x16=GET_FEAT_FAIL, 0x41=FLSH_INFO_ERR
*/
UINT8 flash_info_command(UINT8 *ucCmdDatbuf, UINT8 cmd)
{
   
     if (!ucCmdDatbuf)                                              //sanity check on the buffer
        return FLSH_INFO_ERR;

    /* Form the SET FEAT packet */
    ucCmdDatbuf[0] = FLASH_UPDATE_RPT_ID;                           // set the report id
    ucCmdDatbuf[1] = 0x12;                                          // Data length LSB
    ucCmdDatbuf[2] = 0;                                             // Data length MSB
    ucCmdDatbuf[3] = FLASH_UPDATE_RPT_ID;                           // Report ID
    ucCmdDatbuf[4] = FLASH_INFO_CMD;                                // flash update command
    
    ucCmdDatbuf[5] = cmd;                                           //Reset or BtLdr Info command

    if ( HOST_SF_LIB_HID_Set_Report(SET_RPT_FEAT, ucCmdDatbuf, 5))  //send parameters to SSC7150 via Set Feature command packet
        return SET_FEAT_FAIL;

    //Get the response from the device    
    ucCmdDatbuf[0] = FLASH_UPDATE_RPT_ID;                           // set the report id
    
    if ( HOST_SF_LIB_HID_Get_Report(GET_RPT_FEAT, ucCmdDatbuf, 0) )  //get response parameters from SSC7150 via Get Feature 
        return GET_FEAT_FAIL;

        
    if (ucCmdDatbuf[9] == TRUE)                                     // Check the status code in returned packet
        return SUCCESS;

    else
    {
        // If any error
        memcpy(ucCmdDatbuf, ErrorString[ucCmdDatbuf[10] - 0xA1], 50);
        return FLSH_INFO_ERR;
    }        
}

/** flash_write_command
* @note        Implements flash write command during flash update
* @param ucCmdDatbuf data buffer to write to device 
* @return completion_status 0=SUCCESS, failcode: 0x19=SET_RPT_FAIL,0x41=FLSH_WRITE_ERR
*/
UINT8 flash_write_command(UINT8 *ucCmdDatbuf)
{
    UINT8 i, length;    
    UINT16 chksum = 0;
        UINT8 hid_i2c_rpt_buf[BUF_150];    

    if (!ucCmdDatbuf)                                               //sanity check on the buffer
        return FLSH_WRITE_ERR;

    seq_number++;
    
    // Form the SET FEAT packet 
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id
    hid_i2c_rpt_buf[1] = 0x8F;                                      // Data length LSB
    hid_i2c_rpt_buf[2] = 0;                                         // Data length MSB
    hid_i2c_rpt_buf[3] = FLASH_UPDATE_RPT_ID;                       // Report ID
    hid_i2c_rpt_buf[4] = FLASH_WRITE_CMD;                           //flash update command
    hid_i2c_rpt_buf[5] = seq_number & 0x00FF;                       // sequence number LSB
    hid_i2c_rpt_buf[6] = (seq_number & 0xFF00) >> 8;                // sequence number MSB
    hid_i2c_rpt_buf[7] = 0;
    hid_i2c_rpt_buf[8] = 0;

    // byte 7 & 8 are checksum values, data starts from byte 9 
    memcpy(&hid_i2c_rpt_buf[FLSH_WR_DATA_INDEX], ucCmdDatbuf, PACKET_LENGTH);

    length = FLSH_WR_DATA_INDEX + PACKET_LENGTH;

    // For first packet we need to add total packet numbers 
    if (seq_number == 1)
    {
        hid_i2c_rpt_buf[++length] = TOTAL_64BYTE_PKTS & 0x00FF;         // sequence number LSB
        hid_i2c_rpt_buf[++length] = (TOTAL_64BYTE_PKTS & 0xFF00) >> 8;  // sequence number MSB
    
        // First packet contains CRC of unencrypted image we need to store it for verification purpose in future
        image_crc = ucCmdDatbuf[19];
        image_crc = ((image_crc << 8) | ucCmdDatbuf[18]);
        image_crc = ((image_crc << 8) | ucCmdDatbuf[17]);
        image_crc = ((image_crc << 8) | ucCmdDatbuf[16]);
    }

    // Calculate checksum
    for(i=3; i<=(length); i++)
    {
        chksum += hid_i2c_rpt_buf[i];
    }

    // Append checksum
    hid_i2c_rpt_buf[7] = chksum & 0x00FF;                           // checkkum LSB
    hid_i2c_rpt_buf[8] = (chksum & 0xFF00) >> 8;                    // checksum MSB

    // Send SET OUT Report to module 
    if (HOST_SF_LIB_HID_Set_Report(SET_RPT_OUT, hid_i2c_rpt_buf, length))    
    {
        memcpy(ucCmdDatbuf, "SET_RPT_FAIL", 12);                    //return error string
        return SET_RPT_FAIL;
    }

    //Get the response from the device   
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id again as it is overwritten by last transaction

    
    if ( HOST_SF_LIB_HID_Get_Report(GET_RPT_FEAT, hid_i2c_rpt_buf,0))
    {
        memcpy(ucCmdDatbuf, "SET_RPT2_FAIL", 12);                   //return error string
        return SET_RPT_FAIL;
    }

    // Check the status code 
    if (hid_i2c_rpt_buf[5] == TRUE)    
    {
        sprintf((char *)ucCmdDatbuf, "%d", seq_number);
        
        if (seq_number >= (TOTAL_64BYTE_PKTS/2))                    // If all packets sent 
            img_wr_done = TRUE;
        return SUCCESS;
    }
    else
    {           //error
        seq_number = 1;
        memcpy(ucCmdDatbuf, ErrorString[hid_i2c_rpt_buf[6] - 0xA1], 50); //get error msg from F/W
        return FLSH_WRITE_ERR;
    } 
}


/** flash_verify_command
* @note        Implements flash verify command during flash update
* @param ucCmdDatbuf contains the CRC received from image
* @return completion_status 0=SUCCESS, failcode: 0x1C=HID_GET_RPT_INPT_FAIL,0x19=SET_RPT_FAIL,0x43=FLSH_VERIFY_ERR,0x44=FLSH_CRC_ERR
*/
UINT8 flash_verify_command(UINT8 *ucCmdDatbuf)
{
    UINT32 prgm_crc;
        UINT8 hid_i2c_rpt_buf[BUF_150];    
    
   
    // Form the SET FEAT packet 
    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id
    hid_i2c_rpt_buf[1] = 18;                                        // Data length lower byte
    hid_i2c_rpt_buf[2] = 0;                                         // Data length upper byte
    hid_i2c_rpt_buf[3] = FLASH_UPDATE_RPT_ID;                       // Report ID
    hid_i2c_rpt_buf[4] = FLASH_READ_CMD;                            // Actual flash update command

    if (HOST_SF_LIB_HID_Set_Report(SET_RPT_FEAT, hid_i2c_rpt_buf, 4))
      return SET_RPT_FAIL;

    hid_i2c_rpt_buf[0] = FLASH_UPDATE_RPT_ID;                       // set the report id again as it is overwritten by last transaction

    if ( HOST_SF_LIB_HID_Get_Report(GET_RRT_INPT, hid_i2c_rpt_buf,0))
        return HID_GET_RPT_INPT_FAIL;

    if (hid_i2c_rpt_buf[5] == TRUE)
    {
        prgm_crc = hid_i2c_rpt_buf[12];
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[11]);
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[10]);
        prgm_crc = ((prgm_crc << 8) | hid_i2c_rpt_buf[9]);

        if (image_crc == prgm_crc)
            return SUCCESS;
        else
            return FLSH_CRC_ERR;                                    //CRC_ERROR
    }
    
    else
    {
        // If any error
        memcpy(ucCmdDatbuf, ErrorString[hid_i2c_rpt_buf[10] - 0xA1], 50);
        return FLSH_VERIFY_ERR;
    }        
}
