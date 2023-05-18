#include "app.h"

static double prev_dat[4] = {0};                                  // Used to hold previous data readings to preclude redundant data output
extern char flag;
extern char state;

void _accel() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Accelerometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_ACC_EN))) // Enable the accelerometer (ACC_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACSEN, 0x14)))   // Change Accelerometer sensitivity
    error_handler("VRWr", VREG_ACSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACDXR, 0x64))) // Change acc data rate
    error_handler("VRWr", VREG_ACDXR, ucRetStat);     // error display

  dMult = ret_exponent(ACCEL_VREG_OFFSET);            // Get unit exponent for this sensor's data
    
    // get acc readings
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACXD, &wdata)))  // Read data from the accelerometer datax register
      error_handler("VRRd", VREG_ACXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACYD, &wdata)))  // Read data from the accelerometer datay register
      error_handler("VRRd", VREG_ACYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ACZD, &wdata)))  // Read data from the accelerometer dataz register
      error_handler("VRRd", VREG_ACZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }   //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR
/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      //Disable the Accelerometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_ACC_EN))) // Disable the accelerometer (clear the ACC_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat); // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat);   // error display
    }
}

void _gyro() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Gyrometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_GYRO_EN))) // Enable the Gyrometer (GYRO_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYSEN, 0x01)))  // Change gyro sensitivity
    error_handler("VRWr", VREG_GYSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYDR, 0x64)))    // Change gyro data rate
    error_handler("VRWr", VREG_GYDR, ucRetStat);      // error display

  dMult = ret_exponent(GYRO_VREG_OFFSET);             // Get unit exponent for this sensor's data

    // get gyro readings
    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYXD, &wdata)))  // Read data from the gyrometer datax register
      error_handler("VRRd", VREG_GYXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYYD, &wdata)))  // Read data from the gyrometer datay register
      error_handler("VRRd", VREG_GYYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_GYZD, &wdata)))  // Read data from the gyrometer dataz register
      error_handler("VRRd", VREG_GYZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }   //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      //Disable the Gyrometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_GYRO_EN))) // Disable the Gyrometer (clear the GYRO_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat); // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

void _compass() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Compass
  // To support multi-threaded applications, Read-Modify-Write the VREG :
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_CMP_EN))) // Enable the compass (CMP_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMSEN, 0x01)))   // Change CMP sensitivity
    error_handler("VRWr", VREG_CMSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMDR, 0x64)))    // Change CMP data rate
    error_handler("VRWr", VREG_CMDR, ucRetStat);      // error display

  dMult = ret_exponent(CMP_VREG_OFFSET);              // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_CMD, &wdata))) // Read data from the compass data register
      error_handler("VRRd", VREG_CMD, ucRetStat);   // error display

    dDataX = (int16_t)wdata * dMult;                    // Note: for the compass there is only one data field to be retrieved

    if (prev_dat[0] != dDataX)                      // Ensure that this data has not yet been displayed on LCD
    {
      prev_dat[0] = dDataX;                       // Store the current data value for later reference
      //sprintf(_x, "%4.3f", dDataX);
    }   //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

    //sprintf(_x, "%4.3f", dDataX);

    if (flag)
    {
      flag = 0;
      //Disable the Compass on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_CMP_EN))) // Disable the compass (clear the CMP_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

void _orient() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Orientation sensor
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_ORI_EN))) // Enable the orientation (ORI_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ORSEN, 0xC8)))    // Change ORI sensitivity
    error_handler("VRWr", VREG_ORSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ORDR, 0x32)))     // Change ORI data rate
    error_handler("VRWr", VREG_ORDR, ucRetStat);      // error display

  dMult = ret_exponent(ORI_VREG_OFFSET);              // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORXD, &wdata))) // Read data from the orientation sensor datax register
      error_handler("VRRd", VREG_ORXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORYD, &wdata))) // Read data from the orientation sensor datay register
      error_handler("VRRd", VREG_ORYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORZD, &wdata))) // Read data from the orientation sensor dataz register
      error_handler("VRRd", VREG_ORZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_ORWD, &wdata)))  // Read data from the orientation sensor dataw register
      error_handler("VRRd", VREG_ORWD, ucRetStat);  //error display

    dDataW = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ || prev_dat[3] != dDataW) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
      prev_dat[3] = dDataW;
    }   //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR
/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
    sprintf(_w, "%4.3f", dDataW);
*/
    
    if (flag)
    {
      flag = 0;
      //Disable the orientation sensor on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_ORI_EN))) // Disable the orientation sensor (clear the ORI_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

void _inclin() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Inclinometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_INC_EN))) // Enable the Inclinometer (INC_EN)
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_INSEN, 0x01)))    // Change sensitivity
    error_handler("VRWr", VREG_INSEN, ucRetStat);     // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_INDR, 0x32)))    // Change data rate
    error_handler("VRWr", VREG_INDR, ucRetStat);      // error display

  dMult = ret_exponent(INCL_VREG_OFFSET);             // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INXD, &wdata))) // Read data from the inclinometer datax register
      error_handler("VRRd", VREG_INXD, ucRetStat);  // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INYD, &wdata)))  // Read data from the inclinometer datay register
      error_handler("VRRd", VREG_INYD, ucRetStat);  // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_INZD, &wdata)))  // Read data from the inclinometer dataz register
      error_handler("VRRd", VREG_INZD, ucRetStat);  // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }     //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      // Disable the Inclinometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~VREG_SHC_INC_EN))) // Disable the Inclinometer (clear the INC_EN bit)
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

  char _x[11], _y[11], _z[11], _w[11];

void _rawAccel() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

    //LATG.B0 = !LATG.B0;

  // Enable the Raw Accelerometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWACC_EN | VREG_SHC_ACC_EN))) // Enable the raw accelerometer (RAWACC_EN)& corresponding ACCEL sensor
                                                       //rev 0.10 spec : 'for any raw sensor to be enabled properly, the corresponding
                                                       //sensors must also be enabled. For RAW ACCEL corresponding sensor : ACCEL

      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else  {
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display
  }

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RACSEN, RAW_SENSITIVITY_MIN))) // Set Raw Acc (and corresponding sensor (Acc)) sensitivity to "0" to
                                        //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_ACSEN, RAW_SENSITIVITY_MIN))) // Set Acc sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_ACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RACDR, RAW_DATA_RATE))) // Change Raw Acc data rate
    error_handler("VRWr", VREG_RACDR, ucRetStat);     //error display

  dMult = ret_exponent(RAW_ACC_VREG_OFFSET);          // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACXD, &wdata)))  // Read data from the raw accelerometer datax register
      error_handler("VRRd", VREG_RACXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACYD, &wdata))) // Read data from the raw accelerometer datay register
      error_handler("VRRd", VREG_RACYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RACZD, &wdata)))  // Read data from the raw accelerometer dataz register
      error_handler("VRRd", VREG_RACZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                           // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }    //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      // Disable the Raw Accelerometer & corresponding ACCEL sensor on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWACC_EN | VREG_SHC_ACC_EN)))) // Disable the raw accelerometer (clear the RAWACC_EN bit) & corresponding ACC_EN
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

void _rawMagnet() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Raw Magnometer
  // To support multi-threaded applications, Read-Modify-Write the VREG :
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWMAG_EN | VREG_SHC_CMP_EN))) // Enable the  Raw Magnometer (RAWMAG_EN) & corresponding CMP sensor
                                                       //rev 0.10 spec: 'for any raw sensor to be enabled properly, the corresponding
                                                       //sensors must also be enabled. For RAW MAG corresponding sensor : CMP
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RMGSEN, RAW_SENSITIVITY_MIN)))  // Set Raw Acc (and corresponding sensor (Cmp)) sensitivity to "0" to
                                         //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RMGSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_CMSEN, RAW_SENSITIVITY_MIN))) // Set Cmp sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_CMSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RMGDR, RAW_DATA_RATE)))  // Change data rate
    error_handler("VRWr", VREG_RMGDR, ucRetStat);     // error display

  dMult = ret_exponent(RAW_MAG_VREG_OFFSET);          // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGXD, &wdata)))  // Read data from the raw magnometer datax register
      error_handler("VRRd", VREG_RMGXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGYD, &wdata)))  // Read data from the raw magnometer datay register
      error_handler("VRRd", VREG_RMGYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RMGZD, &wdata)))  // Read data from the raw magnometer dataz register
      error_handler("VRRd", VREG_RMGZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }    //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      //Disable the Raw Magnometer on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWMAG_EN | VREG_SHC_CMP_EN)))) // Disable the Raw Magnometer (clear the RAWMAG_EN bit) & corresponding CMP_EN
          error_handler("VRWr", VREG_SHC, ucRetStat);      // error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}

void _rawGyro() {
  UINT8 ucRetStat;                                                // return status value
  UINT16 wdata;                                                   // pre-exponent-scaled data
  double dDataX = 0, dDataY = 0, dDataZ = 0, dDataW = 0;          // Data field variables
  double dMult = 1;                                               // Multiplier for data fields : sets them to appropriate order of magnitude
  UINT16 wRMWdata;                                                // temporary data for Read-Modify-Write VREG data

  char _x[15], _y[15], _z[15], _w[15];

  // Enable the Raw Gyrometer
  // To support multi-threaded applications, Read-Modify-Write the VREG:
  if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
  {
    if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata | VREG_SHC_RAWGYR_EN | VREG_SHC_GYRO_EN))) // Enable the Raw Gyrometer (GYR_EN) & corresponding GYRO sensor
                                                        //rev 0.10 spec: 'for any raw sensor to be enabled properly, the corresponding
                                                        //sensors must also be enabled. For RAW GYRO corresponding sensor : GYRO
      error_handler("VRWr", VREG_SHC, ucRetStat);  // error display
  }
  else
    error_handler("VRRd", VREG_SHC, ucRetStat);       // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RGYSEN, RAW_SENSITIVITY_MIN))) // Set Raw Gyro (and corresponding sensor (Gyro)) sensitivity to "0" to
                                        //achieve the desired data rate for raw sensors (see spec).
    error_handler("VRWr", VREG_RACSEN, ucRetStat);    // error display

  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_GYSEN, RAW_SENSITIVITY_MIN))) // Set Gyro sensitivity to "0" to achieve the desired data rates for raw sensors.
    error_handler("VRWr", VREG_GYSEN, ucRetStat);    // error display


  if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_RGYDR, RAW_DATA_RATE)))  // Change data rate
    error_handler("VRRd", VREG_RGYDR, ucRetStat);     // error display

  dMult = ret_exponent(RAW_GYR_VREG_OFFSET);          // Get unit exponent for this sensor's data

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYXD, &wdata))) // Read data from the raw gyrometer datax register
      error_handler("VRRd", VREG_RGYXD, ucRetStat); // error display

    dDataX = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYYD, &wdata))) // Read data from the raw gyrometer datay register
      error_handler("VRRd", VREG_RGYYD, ucRetStat); // error display

    dDataY = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_RGYZD, &wdata))) // Read data from the raw gyrometer dataz register
      error_handler("VRRd", VREG_RGYZD, ucRetStat); // error display

    dDataZ = (int16_t)wdata * dMult;                    // Multiply this by the exponent scaler to get floating point value

    if (prev_dat[0] != dDataX || prev_dat[1] != dDataY || prev_dat[2] != dDataZ) // Ensure that this data has not yet been displayed
    {
      prev_dat[0] = dDataX;                       // Store the current data values for later reference
      prev_dat[1] = dDataY;
      prev_dat[2] = dDataZ;
    }   //NOTE: If data does not change in response to movement of sensor, SSC7150 not sending EC_DATA_AVAIL interrupts and 'may' need POR

/*
    sprintf(_x, "%4.3f", dDataX);
    sprintf(_y, "%4.3f", dDataY);
    sprintf(_z, "%4.3f", dDataZ);
*/

    if (flag)
    {
      flag = 0;
      //Disable the Raw Gyro on exit
      if ((ucRetStat = HOST_SF_LIB_VREG_read(VREG_SHC, &wRMWdata)) == SUCCESS)  // Read data from the Sensor Hub Configuration register
      {
        if ((ucRetStat = HOST_SF_LIB_VREG_write(VREG_SHC, wRMWdata & ~(VREG_SHC_RAWGYR_EN | VREG_SHC_GYRO_EN)))) // Disable the raw gyro (clear the GYR_EN bit) & corresponding GYRO_EN
          error_handler("VRWr", VREG_SHC, ucRetStat);      //error display
      }
      else
        error_handler("VRRd", VREG_SHC, ucRetStat); // error display
    }
}
