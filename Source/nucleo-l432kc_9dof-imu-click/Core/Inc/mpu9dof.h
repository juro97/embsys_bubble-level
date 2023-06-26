#ifndef MPU9DOF_H
#define MPU9DOF_H

#include "main.h"


#ifdef __cplusplus
extern "C"{
#endif

void mpu9dof_default_cfg();
void mpu9dof_read(const uint8_t *sensor);
void calibrateRawValues();
void mpu9dof_calibrate_module(void);

/* return a pointer to the data struct */
void *mpu9dof_process(void);

void mpu9dof_log_data(const uint8_t *data );
void mpu9dof_generic_write (const uint8_t *reg, const uint8_t *data_buf, const uint8_t *len );
void mpu9dof_generic_read(const uint8_t *reg, uint8_t *data_buf, const uint8_t *len);
typedef struct ProcessedData
{

	int16_t roll;
	int16_t pitch;

	uint8_t shake;
} ProcessedData;


#ifdef __cplusplus
}
#endif
#endif  // _MPU9DOF_H_

