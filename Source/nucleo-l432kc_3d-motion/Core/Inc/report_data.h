#ifndef INC_REPORT_DATA_H_
#define INC_REPORT_DATA_H_

#include <stdbool.h>

/**
 * This struct is send to the broker
 */
typedef struct sensor_data {
	uint8_t x;
	uint8_t y;
	bool shake;
} report;

#endif /* INC_REPORT_DATA_H_ */
