#ifndef SENSORS_H
#define SENSORS_H

void _accel();
void _compass();
void _gyro();
void _inclin();
void _orient();
void _rawAccel();
void _rawGyro();
void _rawMagnet();

static volatile uint8_t x = 0;
static volatile uint8_t y = 0;
static volatile uint8_t z = 0;

#endif
