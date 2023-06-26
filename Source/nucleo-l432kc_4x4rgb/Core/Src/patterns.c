#include "patterns.h"

/**
    @brief CCR Register Values for PWM Duty Cycle.
    */
#define ONE 	57
#define ZERO	31


/**
    @brief Predefined Colors for one LED.
    */

const rgb_led off =
{
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
	{ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO,ZERO},
};

const rgb_led all_on =
{
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ONE, ONE, ONE, ONE, ONE, ONE, ONE},
};

const rgb_led red =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ONE,  ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led green =
{
	{ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led blue =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE},
};


const rgb_led yellow =
{
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},
};

const rgb_led orange =
{
	{ZERO, ZERO, ONE, ZERO, ZERO, ONE, ZERO, ONE},
	{ZERO, ZERO, ONE, ONE, ONE, ONE, ONE, ONE},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO},

};

const rgb_led white =
{
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
	{ZERO, ZERO, ZERO, ZERO, ZERO, ONE, ZERO, ZERO},
};

/**

    @brief Predefined Patterns for the Matrix.
    */

const rgb_pattern rgb4x4click_centered =
{{
	white, 	white, 	white, 	white,
	white, 	green, 	green, 	white,
	white, 	green, 	green, 	white,
	white, 	white, 	white,	white
}};

const rgb_pattern rgb4x4click_outOfScope =
{{
	red, 	red, 	red, 	red,
	red, 	white,	white,	red,
	red, 	white,	white,	red,
	red, 	red, 	red, 	red,
}};

const rgb_pattern rgb4x4click_shakeDetection =
{{
	blue,	off, 	off, 	blue,
	off, 	blue, 	blue, 	off,
	off, 	blue, 	blue, 	off,
	blue, 	off, 	off, 	blue
}};

const rgb_pattern rgb4x4click_lightShift_lu =
{{
	yellow, yellow, yellow, off,
	yellow, green, 	yellow, off,
	yellow, yellow, yellow, off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_lightShift_lo =
{{
	off, 	off, 	off, 	off,
	yellow, yellow, yellow, off,
	yellow, green, 	yellow, off,
	yellow, yellow, yellow,	off
}};

const rgb_pattern rgb4x4click_lightShift_ru =
{{
	off, 	yellow, yellow, yellow,
	off, 	yellow, green, 	yellow,
	off, 	yellow, yellow, yellow,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_lightShift_ro =
{{
	off, 	off, 	off, 	off,
	off, 	yellow, yellow, yellow,
	off, 	yellow, green, 	yellow,
	off, 	yellow, yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r1c1 =
{{
	red,	yellow,	off,	off,
	yellow,	yellow,	off,	off,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c2 =
{{
	yellow,	red, 	yellow, off,
	yellow,	yellow,	yellow,	off,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c3 =
{{
	off,	yellow,	red,	yellow,
	off,	yellow,	yellow,	yellow,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r1c4 =
{{
	off, 	off, 	yellow,	red,
	off, 	off,	yellow,	yellow,
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c1 =
{{
	yellow,	yellow,	off,	off,
	red,	yellow,	off,	off,
	yellow,	yellow,	off,	off,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r2c4 =
{{
	off, 	off, 	yellow, yellow,
	off,	off, 	yellow, red,
	off, 	off, 	yellow, yellow,
	off, 	off, 	off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r3c1 =
{{
	off,	off, 	off, 	off,
	yellow, yellow, off, 	off,
	red, 	yellow, off, 	off,
	yellow, yellow, off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r3c4 =
{{
	off,	off,	off,	off,
	off,	off, 	yellow,	yellow,
	off,	off,	yellow, red,
	off,	off,	yellow, yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c1 =
{{
	off,	off, 	off, 	off,
	off, 	off, 	off, 	off,
	yellow, yellow, off, 	off,
	red, 	yellow, off, 	off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c2 =
{{
	off, 	off, 	off, 	off,
	off, 	off, 	off, 	off,
	yellow, yellow, yellow, off,
	yellow, red, 	yellow, off,
}};

const rgb_pattern rgb4x4click_strongShift_r4c3 =
{{
	off,	off,	off,	off,
	off,	off, 	off, 	off,
	off,	yellow, yellow, yellow,
	off,	yellow, red,  	yellow,
}};

const rgb_pattern rgb4x4click_strongShift_r4c4 =
{{
	off,	off, 	off,	off,
	off,	off, 	off, 	off,
	off,	off, 	yellow, yellow,
	off, 	off,	yellow, red
}};

const rgb_pattern rgb4x4click_error =
{{
	red,	off, 	off,	red,
	off,	off, 	off, 	off,
	off,	off, 	off, 	off,
	red, 	off,	off, 	red,
}};

const rgb_pattern rgb4x4click_pairing =
{{
	off,	off, 	off,	off,
	off,	off, 	off, 	off,
	off,	off, 	yellow, yellow,
	off, 	off,	yellow, red
}};

const rgb_pattern rgb4x4click_all_off =
{{
	off, 	off, 	off, 	off,
	off, 	off,	off, 	off,
	off, 	off, 	off, 	off,
}};

