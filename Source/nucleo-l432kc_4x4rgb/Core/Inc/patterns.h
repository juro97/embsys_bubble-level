#ifndef INC_PATTERNS_H_
#define INC_PATTERNS_H_


#include <stdio.h>

/* Datatype for one LED */
typedef struct __attribute__((__packed__))
{
	uint8_t green[8];
	uint8_t red[8];
	uint8_t blue[8];
} rgb_led;


/* Datatype for full 4x4 Matrix */
typedef struct  __attribute__((__packed__))
{
	rgb_led led[16];
} rgb_pattern;


/* Predefined Patterns for the Matrix */
extern const rgb_pattern rgb4x4click_all_off;
extern const rgb_pattern rgb4x4click_centered;
extern const rgb_pattern rgb4x4click_outOfScope;
extern const rgb_pattern rgb4x4click_shakeDetection;
extern const rgb_pattern rgb4x4click_lightShift_lu;
extern const rgb_pattern rgb4x4click_lightShift_lo;
extern const rgb_pattern rgb4x4click_lightShift_ru;
extern const rgb_pattern rgb4x4click_lightShift_ro;
extern const rgb_pattern rgb4x4click_strongShift_r1c1;
extern const rgb_pattern rgb4x4click_strongShift_r1c2;
extern const rgb_pattern rgb4x4click_strongShift_r1c3;
extern const rgb_pattern rgb4x4click_strongShift_r1c4;
extern const rgb_pattern rgb4x4click_strongShift_r2c1;
extern const rgb_pattern rgb4x4click_strongShift_r2c4;
extern const rgb_pattern rgb4x4click_strongShift_r3c1;
extern const rgb_pattern rgb4x4click_strongShift_r3c4;
extern const rgb_pattern rgb4x4click_strongShift_r4c1;
extern const rgb_pattern rgb4x4click_strongShift_r4c2;
extern const rgb_pattern rgb4x4click_strongShift_r4c3;
extern const rgb_pattern rgb4x4click_strongShift_r4c4;
extern const rgb_pattern rgb4x4click_error;
extern const rgb_pattern rgb4x4click_pairing;

#endif /* INC_PATTERNS_H_ */
