#include "app.h"
#include "main.h"
#include "cmsis_os.h"

extern volatile BOOL TIMER_1MS_FLG;
extern volatile BOOL EC_DATA_AVAIL;                               // HIDI2_HOST_INT indicates EC data available
volatile UINT32 I2C_TIMEOUT_1MS_CNTR = 0;

void IntPinHandler() iv IVT_EXTERNAL_1 ilevel 7 ics ICS_SRS{
  if (IFS0bits.INT1IF)                                            // Check the HID_I2C_ALERT (INT1) interrupt : indicates data from EC available
  {
    if (!EC_DATA_AVAIL)                                           // If a falling edge occurred (data is available from EC)
    {
      INTCONbits.INT1EP = 1;                                      // INT1 Edge configured to interrupt on rising edge (wait for end of data)
      EC_DATA_AVAIL = TRUE;                                       // Toggle EC_DATA_AVAIL flag to notify data received
    }
    else
    {
      INTCONbits.INT1EP = 0;                                      // INT1 Edge configured to interrupt on falling edge (data is no longer available)
      EC_DATA_AVAIL = FALSE;                                      // interrupt de-asserted
    }

    IFS0bits.INT1IF = 0;                                          // Clear int flag
  }
}

void TimerHandler() iv IVT_TIMER_1 ilevel 7 ics ICS_AUTO{
  IFS0bits.T1IF = 0;                                              // Clear IF bit
  TIMER_1MS_FLG = 1;                                              // 1 ms flag
}

void InitTimer() {
  T1CON = 0x8010;
  T1IE_bit = 1;
  T1IF_bit = 0;
  T1IP0_bit = 1;
  T1IP1_bit = 1;
  T1IP2_bit = 1;
  PR1 = 10000;
  TMR1 = 0;
}

void InitI2CTimer() {
  T2CON = 0x0010;
  T2IE_bit = 1;
  T2IF_bit = 0;
  T2IP0_bit = 1;
  T2IP1_bit = 1;
  T2IP2_bit = 1;
  PR2 = 40000;
  TMR2 = 0;
}

void Timer2Interrupt() iv IVT_TIMER_2 ilevel 7 ics ICS_SRS{
  T2IF_bit = 0;
  I2C_TIMEOUT_1MS_CNTR++;                                         // increment global count val

  if (I2C_TIMEOUT_1MS_CNTR >= I2_TIMEOUT_PERIOD)
  {                                                               // looks like i2c taking too long
    StopI2CTimer();                                               // turn off timer
    error_handler("i2c ",0, I2C_TIMEOUT_ERR);                     // displays to LCD and uart **does not return***
  }
}

void StopI2CTimer() {
  T2CONbits.ON = 0;
}

void StartI2CTimer() {
  I2C_TIMEOUT_1MS_CNTR = 0;
  T2CONbits.ON = 1;
}
