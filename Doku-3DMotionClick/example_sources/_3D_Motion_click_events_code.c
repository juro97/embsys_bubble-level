#include "_3D_Motion_click_objects.h"
#include "_3D_Motion_click_resources.h"

//--------------------- User code ---------------------//
extern unsigned char state;
//----------------- End of User code ------------------//

// Event Handlers

char flag = 0;

void BackOnClick() {
  flag = 1;
  state = 0;
  DrawScreen(&SelectSensor);
}

void CircleButton1OnClick() {
  DrawScreen(&RawAccel);
  state = 1;
}

void Button1OnClick() {
  DrawScreen(&SelectSensor);
}

void CircleButton2OnClick() {
  DrawScreen(&RawGyro);
  state = 2;
}

void CircleButton3OnClick() {
  DrawScreen(&RawMagnet);
  state = 3;
}

void CircleButton4OnClick() {
  DrawScreen(&Accel);
  state = 4;
}

void CircleButton5OnClick() {
  DrawScreen(&Compass);
  state = 5;
}

void CircleButton6OnClick() {
  DrawScreen(&Gyro);
  state = 6;
}

void CircleButton7OnClick() {
  DrawScreen(&Inclinometer);
  state = 7;
}

void CircleButton8OnClick() {
  DrawScreen(&Orientation);
  state = 8;
}
