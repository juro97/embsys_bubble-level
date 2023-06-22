#ifndef INC_REPORT_DATA_H_
#define INC_REPORT_DATA_H_

#include "main.h"

typedef struct ReportData {
	int16_t roll;
	int16_t pitch;
	uint8_t shake;
} ReportData;

#endif /* INC_REPORT_DATA_H_ */
