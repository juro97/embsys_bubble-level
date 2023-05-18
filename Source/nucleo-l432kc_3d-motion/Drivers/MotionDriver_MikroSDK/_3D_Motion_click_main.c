/*
 * Project name:
      3D Motion click (Using MM7150 motion module)
 * Copyright:
     (c) Mikroelektronika, 2015.
 * Revision History:
        20150731:
       - initial release (FJ);
 * Description:
     This is a sample program which demonstrates the usage of the Microchip's MM7150 motion module. 
     This module contains the SSC7150 motion coprocessor with integrated 9-axis sensor fusion 
     as well as high performance MEMS technology including a 3-axis accelerometer, gyroscope and magnetometer.
     Example displays data acquired from the sensors on TFT display.
 * Test configuration:
     MCU:             P32MX795F512L
                      http://ww1.microchip.com/downloads/en/DeviceDoc/61156F.pdf
     Dev.Board:       EasyPIC Fusion v7
                      http://www.mikroe.com/easypic-fusion/
     Oscillator:      XT-PLL, 80.000MHz
     Ext. Modules:    3D Motion click Board - ac:_3D_Motion_click
                      http://www.mikroe.com/click/dac/
     SW:              mikroC PRO for PIC32
                      http://www.mikroe.com/mikroc/pic32/
 * NOTES:
     - Place 3D Motion click board at the mikroBUS socket 1.
     - Place EasyTFT and turn on backlight at SW11.
     - Turn on touch panel switches at SW11.
 */
#include "app.h"

// 3D motion click module connections
sbit WKUP at LATB8_bit;
sbit RST at LATC1_bit;
sbit INT_PIN at RE8_bit;

sbit WKUP_DIR at TRISB8_bit;
sbit RST_DIR at TRISC1_bit;
sbit INT_PIN_DIR at TRISE8_bit;
// 3D motion click module connections

uint8_t state = 0;
uint8_t result;

void main() {
  AD1PCFG = 0xFFFF;                    // Set all pins as digital
  JTAGEN_bit = 0;                      // Disable JTAG

  RST_DIR = 0;                         // Set RST pin as output

  RST = 0;                             // Reset device
  Delay_ms(2);
  RST = 1;

  WKUP_DIR = 0;                        // Set WKUP pin as output
  ODCB8_bit = 0;                       // Push-pull
  WKUP = 1;                            // Wake up device

  INT_PIN_DIR = 1;                     // Set INT pin as input

  IFS0bits.INT1IF = 0;                 // Clear INT1 flag
  IPC1bits.INT1IP = 7;                 // Assign INT1 priority
  IPC1bits.INT1IS = 0;                 // Assign INT1 priority

  INTCONbits.INT1EP = 0;               // INT1 Edge set to interrupt on falling edge initially
  IEC0bits.INT1IE = 1;

  InitTimer();                         // Initialize timer
  InitI2CTimer();                      // Initialize I2C timer

  EnableInterrupts();                  // Enable interrupts
  Delay_ms(100);

  I2C2_Init(100000);                   // Initialize I2C module
  Delay_ms(100);

  result = VREG_init();                // Initialize VREG registers

  // State machine selectec sensor
  while (1) {
    Check_TP();
    switch(state){
      case 1:
        // Select Raw Accel
        _rawAccel();
        break;
      case 2:
        // Select Raw Gyro
        _rawGyro();
        break;
      case 3:
        // Select Raw Magnet
        _rawMagnet();
        break;
      case 4:
        // Select Accel
        _accel();
        break;
      case 5:
        // Select Compass
        _compass();
        break;
      case 6:
        // Select Gyro
        _gyro();
        break;
      case 7:
        // Select Inclinometer
        _inclin();
        break;
      case 8:
        // Select Orientation sensor
        _orient();
        break;
      default:
        break;
    }
  }
}
