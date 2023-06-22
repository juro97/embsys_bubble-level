#include "mpu9dof.h"


extern I2C_HandleTypeDef hi2c1;


/************************************
 *			Const 'Defines'			*
 ************************************/

const uint8_t MPU9DOF_XLG_I2C_ADDR_1 = 0x69;

const uint8_t MPU9DOF_BIT_H_RESET = 0x80;
const uint8_t MPU9DOF_DEFAULT = 0x00;
const uint8_t MPU9DOF_BITS_DLPF_CFG_42HZ = 0x03;
const uint8_t MPU9DOF_BIT_FIFO_DIS = 0x00;
const uint8_t MPU9DOF_BIT_INT_PIN_CFG = 0x02;
const uint8_t MPU9DOF_SMPLRT_DIV = 0x19;
const uint8_t MPU9DOF_CONFIG = 0x1A;
const uint8_t MPU9DOF_GYRO_CONFIG = 0x1B;
const uint8_t MPU9DOF_ACCEL_CONFIG = 0x1C;
const uint8_t MPU9DOF_FIFO_EN = 0x23;
const uint8_t MPU9DOF_INT_PIN_CFG = 0x37;
const uint8_t MPU9DOF_INT_ENABLE = 0x38;
const uint8_t MPU9DOF_USER_CTRL = 0x6A;
const uint8_t MPU9DOF_PWR_MGMT_1 = 0x6B;
const uint8_t MPU9DOF_PWR_MGMT_2 = 0x6C;

/* sensitivity setting for acc and gyro */
const uint8_t MPU9DOF_BITS_FS_250DPS = 0x00;
const uint8_t MPU9DOF_BITS_AFSL_SEL_8G = 0x10;

/* high registers of the gyro and accel registers */
const uint8_t MPU9DOF_GYRO_XOUT_H = 0x43;
const uint8_t MPU9DOF_GYRO_YOUT_H = 0x45;
const uint8_t MPU9DOF_GYRO_ZOUT_H = 0x47;
const uint8_t MPU9DOF_ACCEL_XOUT_H = 0x3B;
const uint8_t MPU9DOF_ACCEL_YOUT_H = 0x3D;
const uint8_t MPU9DOF_ACCEL_ZOUT_H = 0x3F;

const float SHAKE_THRESHOLD 	= 2.5;  // 2.5 means 2.5 G's
const float DELTA_TIME 			= 1.0/30.0;	// read 30 times per second from sensor
const float RAD_TO_DEG 			= 57.2957795131; // conversion constant
const float COMPLEMENTARY_RATIO	= 0.98; // this is from the world wide webs knowledge

/* how many example values should be recorded for the calibration */
const uint8_t CALIBRATION_SAMPLES = 50;


/************************************
 *			Data Definition			*
 ************************************/

typedef struct RawData
{
	/* Accelerometer */
	int16_t acc[3];

	/* Gyroscope */
	int16_t gyro[3];

} RawData;


typedef struct OffsetValues
{
	/* offset values for the gyro */
	int16_t gyro_offset[3];

	/* offset values for the accel */
	int16_t acc_offset[3];

} OffsetValues;


typedef struct CalibratedData
{
	/* Accelerometer */
	int16_t acc_calib[3];

	/* Gyroscope */
	int16_t gyro_calib[3];

} CalibratedData;


typedef struct ProcessedData
{
	/* Bubble Level */
	int16_t roll;
	int16_t pitch;

	/* Shake Detection */
	uint8_t shake;
} ProcessedData;


RawData rawData;
ProcessedData processedData;
OffsetValues offsetValues;
CalibratedData calibratedData;

/************************************
 *		Method Implementation		*
 ************************************/

/**
 * @brief:
 * @param:
 * @return:
 */
void mpu9dof_default_cfg()
{

	const uint8_t LEN = 1;

    /* Chip reset */
    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_1, &MPU9DOF_BIT_H_RESET, &LEN );
    HAL_Delay(10);

    /* Initialize accel & gyro */
    mpu9dof_generic_write( &MPU9DOF_SMPLRT_DIV, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_CONFIG, &MPU9DOF_BITS_DLPF_CFG_42HZ, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_GYRO_CONFIG, &MPU9DOF_BITS_FS_250DPS, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_ACCEL_CONFIG, &MPU9DOF_BITS_AFSL_SEL_8G, &LEN );
    HAL_Delay(10);

    /* Disable FIFOs */
    mpu9dof_generic_write( &MPU9DOF_FIFO_EN , &MPU9DOF_BIT_FIFO_DIS, &LEN );
    HAL_Delay(10);

    /* Bypass mode enabled */
    mpu9dof_generic_write( &MPU9DOF_INT_PIN_CFG , &MPU9DOF_BIT_INT_PIN_CFG, &LEN );
    HAL_Delay(10);

    /* Disable all interrupts */
    mpu9dof_generic_write( &MPU9DOF_INT_ENABLE , &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

    /* No FIFO and no I2C slaves */
    mpu9dof_generic_write( &MPU9DOF_USER_CTRL , &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

    /* No power management, internal clock source */
    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_1, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_2, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);
}


/**
 * @brief:
 * @param:
 * @return:
 */
void mpu9dof_read(const uint8_t *sensor)
{
	uint8_t buffer[2] = {0};

	const uint8_t LEN = 2;

	switch(*sensor)
	{
		case 1:
			mpu9dof_generic_read( &MPU9DOF_GYRO_XOUT_H, buffer, &LEN );
			rawData.gyro[0] = (int16_t)(buffer[0] << 8 | buffer[1]);

			mpu9dof_generic_read( &MPU9DOF_GYRO_YOUT_H, buffer, &LEN );
			rawData.gyro[1] = (int16_t)(buffer[0] << 8 | buffer[1]);

			mpu9dof_generic_read( &MPU9DOF_GYRO_ZOUT_H, buffer, &LEN );
			rawData.gyro[2] = (int16_t)(buffer[0] << 8 | buffer[1]);
			break;

		case 2: /* Accel */
			mpu9dof_generic_read( &MPU9DOF_ACCEL_XOUT_H, buffer, &LEN );
		    rawData.acc[0] = (int16_t)(buffer[0] << 8 | buffer[1]);

		    mpu9dof_generic_read( &MPU9DOF_ACCEL_YOUT_H, buffer, &LEN );
		    rawData.acc[1] = (int16_t)(buffer[0] << 8 | buffer[1]);

		    mpu9dof_generic_read( &MPU9DOF_ACCEL_ZOUT_H, buffer, &LEN );
		    rawData.acc[2] = (int16_t)(buffer[0] << 8 | buffer[1]);
			break;

		default:
			printf("passed wrong Argument to function");
	}
}


/**
 * @brief: Calibrates the 9DOF MPU module by summing sample data up and calculating the mean value out of it
 * @param: none
 * @return: none
 */
void mpu9dof_calibrate_module()
{
	const uint8_t GYRO = 1;

	int32_t gyro_sum[3] = {0, 0, 0};

	for (size_t i = 0; i < CALIBRATION_SAMPLES; i++)
	{
	        mpu9dof_read(&GYRO);
	        gyro_sum[0] += rawData.gyro[0];
	        gyro_sum[1] += rawData.gyro[1];
	        gyro_sum[2] += rawData.gyro[2];

	        // Delay to not exceed sensor's max data rate
	        HAL_Delay(10);
	}

    for (size_t i = 0; i < 3; i++)
    {
        offsetValues.gyro_offset[i] = gyro_sum[i] / CALIBRATION_SAMPLES;
        calibratedData.gyro_calib[i] = rawData.gyro[i] - offsetValues.gyro_offset[i];
    }

    /* normal 1G offset of accel in Z-achsis */
    offsetValues.acc_offset[2] -= 16384.0;
    calibratedData.acc_calib[2] = rawData.acc[2] - offsetValues.acc_offset[2];
}

/**
 * @brief:
 * @param:
 * @return:
 */
void *mpu9dof_process()
{
		/* convert raw values to degrees/sec */
		float gyro_x_rate = (rawData.gyro[0] - offsetValues.gyro_offset[0]) / 131.0;
		float gyro_y_rate = (rawData.gyro[1] - offsetValues.gyro_offset[1]) / 131.0;

		/* convert raw values to G's */
		float acc_x = rawData.acc[0] / 4096.0;
		float acc_y = rawData.acc[1] / 4096.0;
		float acc_z = rawData.acc[2] / 4096.0;

		/* calculate angle from accelerometer data */
		float roll_acc = atan2f(acc_x, acc_z) * RAD_TO_DEG;
		float pitch_acc = atan2f(acc_y, acc_z) * RAD_TO_DEG;

		/* apply the complementary filter */
		processedData.roll = COMPLEMENTARY_RATIO * (processedData.roll - gyro_y_rate * DELTA_TIME) + (1 - COMPLEMENTARY_RATIO) * roll_acc;
		processedData.pitch = COMPLEMENTARY_RATIO * (processedData.pitch + gyro_x_rate * DELTA_TIME) + (1 - COMPLEMENTARY_RATIO) * pitch_acc;

		/* shake detection */
		float magnitude = sqrtf(acc_x * acc_x + acc_y * acc_y + acc_z * acc_z);


		if (magnitude > SHAKE_THRESHOLD)
		{
			processedData.shake = 0;
		}
		else
		{
			processedData.shake = 1;
		}
		return &processedData;
}


/**
 * @brief:
 * @param:
 * @return:
 */
void mpu9dof_log_data(const uint8_t *data )
{
	switch(*data)
	{
		/* log the raw data */
		case 1:
			 printf("raw-acc: x=%d, y=%d, z=%d\r\n", rawData.acc[0], rawData.acc[1], rawData.acc[2]);
			 printf("raw-gyr: x=%d, y=%d, z=%d\r\n", rawData.gyro[0], rawData.gyro[1], rawData.gyro[2]);
			break;

		/* log the calibration offset */
		case 2:
			 printf("offset-gyro: x=%d, y=%d, z=%d\r\n", offsetValues.gyro_offset[0], offsetValues.gyro_offset[1], offsetValues.gyro_offset[2]);
			break;

		/* log the calibrated data */
		case 3:
			/* TODO */
			break;

		/* log the processed final data */
		case 4:
			printf("final: roll=%d, pitch=%d, shake=%d\r\n", processedData.roll, processedData.pitch, processedData.shake);
			break;

		default:
			printf("** ERROR: Passed Wrong Values! **\r\n");
	}
}


/**
 * @brief:
 * @param:
 * @return:
 */
void mpu9dof_generic_write (const uint8_t *reg, const uint8_t *data_buf, const uint8_t *len )
{
	taskENTER_CRITICAL();
	uint8_t data[256] = {0};

	data[0] = *reg;
	memcpy(&data[1], data_buf, *len);

    HAL_I2C_Master_Transmit(&hi2c1, MPU9DOF_XLG_I2C_ADDR_1<<1, data, (*len) + 1, HAL_MAX_DELAY);
    taskEXIT_CRITICAL();
}


/**
 * @brief:
 * @param:
 * @return:
 */
void mpu9dof_generic_read(const uint8_t *reg, uint8_t *data_buf, const uint8_t *len)
{
	taskENTER_CRITICAL();
	HAL_I2C_Mem_Read(&hi2c1, MPU9DOF_XLG_I2C_ADDR_1<<1, *reg, I2C_MEMADD_SIZE_8BIT, data_buf, *len, HAL_MAX_DELAY);
	taskEXIT_CRITICAL();
}
