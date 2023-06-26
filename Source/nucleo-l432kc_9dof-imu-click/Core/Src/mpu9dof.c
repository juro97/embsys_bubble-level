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

/**

@brief Sensitivity settings for accelerometer and gyroscope.
These constants define the sensitivity settings for the accelerometer and gyroscope of the MPU-9DOF sensor.
MPU9DOF_BITS_FS_250DPS: Sensitivity setting for the gyroscope at 250 degrees per second.
MPU9DOF_BITS_AFSL_SEL_8G: Sensitivity setting for the accelerometer at 8g.
*/

const uint8_t MPU9DOF_BITS_FS_250DPS = 0x00;
const uint8_t MPU9DOF_BITS_AFSL_SEL_8G = 0x10;

/**

@brief High registers of the gyroscope and accelerometer.
These constants define the addresses of the high registers of the gyroscope and accelerometer in the MPU-9DOF sensor.
MPU9DOF_GYRO_XOUT_H: Address of the high register for the X-axis gyroscope output.
MPU9DOF_GYRO_YOUT_H: Address of the high register for the Y-axis gyroscope output.
MPU9DOF_GYRO_ZOUT_H: Address of the high register for the Z-axis gyroscope output.
MPU9DOF_ACCEL_XOUT_H: Address of the high register for the X-axis accelerometer output.
MPU9DOF_ACCEL_YOUT_H: Address of the high register for the Y-axis accelerometer output.
MPU9DOF_ACCEL_ZOUT_H: Address of the high register for the Z-axis accelerometer output.
*/
		
const uint8_t MPU9DOF_GYRO_XOUT_H = 0x43;
const uint8_t MPU9DOF_GYRO_YOUT_H = 0x45;
const uint8_t MPU9DOF_GYRO_ZOUT_H = 0x47;
const uint8_t MPU9DOF_ACCEL_XOUT_H = 0x3B;
const uint8_t MPU9DOF_ACCEL_YOUT_H = 0x3D;
const uint8_t MPU9DOF_ACCEL_ZOUT_H = 0x3F;

/**

@brief Constants for motion sensing.
These constants define various parameters used in motion sensing calculations.
SHAKE_THRESHOLD: Threshold for detecting shaking motion, specified in units of G's (acceleration due to gravity).
DELTA_TIME: Time interval between sensor readings, expressed as the reciprocal of the sampling rate (30 times per second).
RAD_TO_DEG: Conversion constant for converting radians to degrees.
COMPLEMENTARY_RATIO: Complementary filter ratio used in sensor fusion algorithms, obtained from external knowledge sources.
*/

const float SHAKE_THRESHOLD 	= 2.5;  
const float DELTA_TIME 			= 1.0/30.0;
const float RAD_TO_DEG 			= 57.2957795131;
const float COMPLEMENTARY_RATIO	= 0.98; 

/**

@brief Specifies the number of example values to be recorded for calibration.
This constant variable determines the number of example values that should be recorded for the calibration process.
It is of type uint8_t, which represents an unsigned 8-bit integer.
The calibration process is used to establish reference values or parameters for calculations.
@note Adjusting this value may impact the accuracy and reliability of the calibration process.
@see CALIBRATION_SAMPLES
*/
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


/**

    @brief Structure representing offset values for gyroscope and accelerometer.
    The OffsetValues struct is used to store the offset values for the gyro and accelerometer. It contains two arrays,
    gyro_offset and acc_offset, which hold the offset values for the gyro and accelerometer. Each array
    has three elements of type int16_t, representing the offset values along the x, y, and z axes.
    These offset values are typically determined during a calibration process and are used to compensate for any biases
    or errors in the sensor readings. By applying the offset values, more accurate and reliable measurements can be obtained.
    @see gyro_offset
    @see acc_offset
    */

typedef struct OffsetValues
{
	int16_t gyro_offset[3];

	int16_t acc_offset[3];

} OffsetValues;

/**

    @brief used to store the calibrated data for the accelerometer and gyroscope
    It contains two arrays, acc_calib and gyro_calib, which hold the calibrated values for the accelerometer and gyroscope,
    respectively. Each array has three elements of type int16_t, representing the calibrated values along the x, y, and z
    axes.
    These calibrated values have been adjusted to remove any biases or errors present in the raw sensor readings. By using
    the calibrated data, more accurate and reliable measurements can be obtained from the accelerometer and gyroscope.
    @see acc_calib
    @see gyro_calib
    */

typedef struct CalibratedData
{
	int16_t acc_calib[3];

	int16_t gyro_calib[3];

} CalibratedData;

/**

    @brief Structure representing processed data including bubble level and shake detection.
    The ProcessedData structure is used to store the processed data, which includes the bubble level readings and shake
    detection result. 
    The roll and pitch variables are of type int16_t and represent the angles of the bubble level along the horizontal
    (roll) and vertical (pitch) axes. These values indicate the orientation or tilt of the object being measured.
    The shake variable is of type uint8_t and represents the shake detection result. It is used to indicate whether a
    shake or vibration event has been detected. A value of 1 indicates a shake or vibration is detected, while a value of
    0 indicates no shake or vibration is detected. 
    @see roll
    @see pitch
    @see shake
    */

typedef struct ProcessedData
{

	int16_t roll;
	int16_t pitch;

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

    /** Chip reset: Resets the chip and adds a 10ms delay. */
    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_1, &MPU9DOF_BIT_H_RESET, &LEN );
    HAL_Delay(10);

	/** Initialize accel & gyro: Sets the sample rate divider to default value and adds a 10ms delay. */
	mpu9dof_generic_write( &MPU9DOF_SMPLRT_DIV, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_CONFIG, &MPU9DOF_BITS_DLPF_CFG_42HZ, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_GYRO_CONFIG, &MPU9DOF_BITS_FS_250DPS, &LEN );
    HAL_Delay(10);

    mpu9dof_generic_write( &MPU9DOF_ACCEL_CONFIG, &MPU9DOF_BITS_AFSL_SEL_8G, &LEN );
    HAL_Delay(10);

    /** Disable FIFOs: Disables the FIFO feature and adds a 10ms delay. */
    mpu9dof_generic_write( &MPU9DOF_FIFO_EN , &MPU9DOF_BIT_FIFO_DIS, &LEN );
    HAL_Delay(10);

	/** Bypass mode enabled: Enables the bypass mode by setting the appropriate configuration in the INT_PIN_CFG register. Adds a 10ms delay. */
    mpu9dof_generic_write( &MPU9DOF_INT_PIN_CFG , &MPU9DOF_BIT_INT_PIN_CFG, &LEN );
    HAL_Delay(10);

	/** Disable all interrupts: Disables all interrupts by writing the default value to the INT_ENABLE register. Adds a 10ms delay. */

    mpu9dof_generic_write( &MPU9DOF_INT_ENABLE , &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

	/** No FIFO and no I2C slaves: Disables the FIFO and I2C slave features by setting the default value in the USER_CTRL register. Adds a 10ms delay. */
    mpu9dof_generic_write( &MPU9DOF_USER_CTRL , &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);

	/** No power management, internal clock source: Disables power management and sets the internal clock source by writing 
	the default value to the PWR_MGMT_1 register. Adds a 10ms delay. */
    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_1, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);



    mpu9dof_generic_write( &MPU9DOF_PWR_MGMT_2, &MPU9DOF_DEFAULT, &LEN );
    HAL_Delay(10);
}


/**

    @brief Read sensor data from MPU9DOF module.

    This function reads sensor data from the MPU9DOF module based on the provided sensor type. The sensor type is specified using the parameter sensor.
    If sensor is equal to 1, the function reads gyro data from the module's registers, including X, Y, and Z-axis gyro values, and stores them in the rawData.gyro array.
    If sensor is equal to 2, the function reads accelerometer data from the module's registers, including X, Y, and Z-axis accelerometer values, and stores them in the rawData.acc array.
    If sensor is neither 1 nor 2, an error message is printed.
    @param sensor Sensor type to read data from. Valid values are 1 (gyro) and 2 (accelerometer).

    @see mpu9dof_generic_read()
    @see MPU9DOF_GYRO_XOUT_H
    @see MPU9DOF_GYRO_YOUT_H
    @see MPU9DOF_GYRO_ZOUT_H
    @see MPU9DOF_ACCEL_XOUT_H
    @see MPU9DOF_ACCEL_YOUT_H
    @see MPU9DOF_ACCEL_ZOUT_H
    @see rawData.gyro
    @see rawData.acc
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

    @brief Calibrates the 9DOF MPU module by calculating the  value of the sample data.
    This function performs calibration for the 9DOF MPU module. The calibration process is performed by reading gyro data using the
    mpu9dof_read() function with the sensor type set to GYRO. The sample data is accumulated in the gyro_sum array.
    After collecting the required number of samples defined by CALIBRATION_SAMPLES, the mean value is calculated for each
    gyro axis. The gyro offset values are stored in the offsetValues.gyro_offset array, and the calibrated gyro data is
    stored in the calibratedData.gyro_calib array.
    Additionally, a normal 1G offset of the accelerometer in the Z-axis is accounted for by subtracting 16384 from the
    offsetValues.acc_offset[2] value. The calibrated accelerometer data is stored in the calibratedData.acc_calib[2]
    variable.

    @note The calibration process assumes that the sensor is at a stable and neutral position during calibration.
    @return None.
    @see mpu9dof_read()
    @see CALIBRATION_SAMPLES
    @see GYRO
    @see gyro_sum
    @see offsetValues.gyro_offset
    @see calibratedData.gyro_calib
    @see offsetValues.acc_offset
    @see calibratedData.acc_calib
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

/** Apply a normal 1G offset correction to the Z-axis accelerometer data. */
	offsetValues.acc_offset[2] -= 16384.0;
    calibratedData.acc_calib[2] = rawData.acc[2] - offsetValues.acc_offset[2];
}


/**

    @brief Process sensor data and calculate roll, pitch, and shake detection.
    This function processes the sensor data and performs calculations to determine the roll, pitch, and shake detection
    values. It takes the raw gyro and accelerometer data from rawData and applies necessary conversions to obtain the
    values in appropriate units.
    The function converts the raw gyro values to degrees per second by subtracting the gyro offset values and dividing by
    131.0. Similarly, the raw accelerometer values are converted to G's by dividing them by 4096.0. The roll and pitch
    angles are calculated from the accelerometer data using the atan2f function and then converted to degrees by
    multiplying with the RAD_TO_DEG constant.
    The complementary filter is applied to combine the gyro and accelerometer data for the roll and pitch angles. The
    filtered roll and pitch values are updated in processedData using the COMPLEMENTARY_RATIO and DELTA_TIME constants.
    The shake detection is performed by calculating the magnitude of the accelerometer readings and comparing it to the
    SHAKE_THRESHOLD. If the magnitude exceeds the threshold, shake is considered to be detected, and the processedData.shake
    value is set to 0; otherwise, it is set to 1.
    The function returns a pointer to the processedData structure containing the calculated roll, pitch, and shake values.

    @return Pointer to the processedData structure.
    @see rawData.gyro
    @see offsetValues.gyro_offset
    @see rawData.acc
    @see COMPLEMENTARY_RATIO
    @see DELTA_TIME
    @see processedData.roll
    @see processedData.pitch
    @see SHAKE_THRESHOLD
    @see processedData.shake
    @see RAD_TO_DEG
    */

void *mpu9dof_process()
{
		float gyro_x_rate = (rawData.gyro[0] - offsetValues.gyro_offset[0]) / 131.0;
		float gyro_y_rate = (rawData.gyro[1] - offsetValues.gyro_offset[1]) / 131.0;

		float acc_x = rawData.acc[0] / 4096.0;
		float acc_y = rawData.acc[1] / 4096.0;
		float acc_z = rawData.acc[2] / 4096.0;

		float roll_acc = atan2f(acc_x, acc_z) * RAD_TO_DEG;
		float pitch_acc = atan2f(acc_y, acc_z) * RAD_TO_DEG;

		processedData.roll = COMPLEMENTARY_RATIO * (processedData.roll - gyro_y_rate * DELTA_TIME) + (1 - COMPLEMENTARY_RATIO) * roll_acc;
		processedData.pitch = COMPLEMENTARY_RATIO * (processedData.pitch + gyro_x_rate * DELTA_TIME) + (1 - COMPLEMENTARY_RATIO) * pitch_acc;

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

    @brief Log data based on the specified type.

    This function logs different types of data based on the value of *data. The function accepts a pointer to the data variable, 
    which is used in a switch statement to determine the type of data to log.
        If *data is 1, the function logs the raw accelerometer and gyroscope data by printing the values of rawData.acc and rawData.gyro using the printf function.
        If *data is 2, the function logs the calibration offset for the gyroscope by printing the values of offsetValues.gyro_offset using the printf function.
        If *data is 3, the function logs the calibrated data. (TODO: Add details if available.)
        If *data is 4, the function logs the processed final data, including the roll, pitch, and shake values from processedData, using the printf function.

    If none of the above cases match, indicating an invalid value for *data, an error message is printed using the printf function.

    @param data Pointer to the data type specifier.
    @return None.
    @see rawData.acc
    @see rawData.gyro
    @see offsetValues.gyro_offset
    @see processedData.roll
    @see processedData.pitch
    @see processedData.shake
    */
void mpu9dof_log_data(const uint8_t *data )
{
	switch(*data)
	{
		case 1:
			 printf("raw-acc: x=%d, y=%d, z=%d\r\n", rawData.acc[0], rawData.acc[1], rawData.acc[2]);
			 printf("raw-gyr: x=%d, y=%d, z=%d\r\n", rawData.gyro[0], rawData.gyro[1], rawData.gyro[2]);
			break;

		case 2:
			 printf("offset-gyro: x=%d, y=%d, z=%d\r\n", offsetValues.gyro_offset[0], offsetValues.gyro_offset[1], offsetValues.gyro_offset[2]);
			break;

		case 3:
			break;

		case 4:
			printf("final: roll=%d, pitch=%d, shake=%d\r\n", processedData.roll, processedData.pitch, processedData.shake);
			break;

		default:
			printf("** ERROR: Passed Wrong Values! **\r\n");
	}
}


/**

    @brief Generic write function for MPU9DOF module.

    This function is used to write data to the MPU9DOF module. It accepts a register address specified by the reg parameter,
    a data buffer specified by the data_buf parameter and the length of the data buffer specified by the len parameter.
    The function begins by acquiring a critical section to ensure exclusive access to shared resources. It then constructs a
    data array with the register address as the first element and copies the data from the data buffer into the remaining
    elements using the memcpy function.
    The function then calls the HAL_I2C_Master_Transmit function to transmit the data array over the I2C interface to the
    MPU9DOF module. The I2C address used for transmission is obtained by left-shifting the constant MPU9DOF_XLG_I2C_ADDR_1 by 1.

    Finally, the function releases the critical section and returns.

    @param reg Pointer to the register address.
    @param data_buf Pointer to the data buffer.
    @param len Pointer to the length of the data buffer.
    @return None.
    @see HAL_I2C_Master_Transmit
    @see memcpy
    @see MPU9DOF_XLG_I2C_ADDR_1
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

    @brief Generic read function for MPU9DOF module.
    This function is used to read data from the MPU9DOF module. It accepts a register address specified by the reg parameter,
    a data buffer specified by the data_buf parameter, and the length of the data buffer specified by the len parameter.
    The function begins by acquiring a critical section to ensure exclusive access to shared resources. It then calls the
    HAL_I2C_Mem_Read function to read data from the MPU9DOF module. The function specifies the I2C interface (hi2c1), the
    I2C device address obtained by left-shifting MPU9DOF_XLG_I2C_ADDR_1 by 1, the register address (*reg), the size of the
    register address (I2C_MEMADD_SIZE_8BIT), the data buffer to store the read data (data_buf), the length of the data to
    be read (*len), and the maximum delay for the operation (HAL_MAX_DELAY).
    Finally, the function releases the critical section and returns.

    @param reg Pointer to the register address.
    @param data_buf Pointer to the data buffer to store the read data.
    @param len Pointer to the length of the data buffer.
    @return None.
    @see HAL_I2C_Mem_Read
    @see MPU9DOF_XLG_I2C_ADDR_1
    */
void mpu9dof_generic_read(const uint8_t *reg, uint8_t *data_buf, const uint8_t *len)
{
	taskENTER_CRITICAL();
	HAL_I2C_Mem_Read(&hi2c1, MPU9DOF_XLG_I2C_ADDR_1<<1, *reg, I2C_MEMADD_SIZE_8BIT, data_buf, *len, HAL_MAX_DELAY);
	taskEXIT_CRITICAL();
}