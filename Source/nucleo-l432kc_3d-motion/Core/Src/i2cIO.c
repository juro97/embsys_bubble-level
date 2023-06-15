#include "app.h"

extern I2C_HandleTypeDef hi2c1;

volatile uint16_t status = 0;
volatile BOOL TIMER_10MS_FLG = FALSE;         // 10 msec timer interrupt flag
volatile BOOL EC_DATA_AVAIL = FALSE;          // EC data available for read flag

void error_handler(char *str, UINT16 offset, UINT16 errval)
{
	printf("\r\n ***Error in function: %s:%02X errval=0x%X*** \r\n", str, offset, errval);

	/*
	Error_Handler();
	 */
}


/** i2c_cmd_WrRd
 * @note        i2c write,read, and combined write/read commands, start timer2 interrupt to exit on unresponsive i2c bus
 * @param ucCmd error code
 * @param ucBytes_wr Number of bytes to write to slave
 * @param ucData_wr Pointer to data buffer to send to slave
 * @param usBytes_rd Number of bytes to read from slave
 * @param ucData_rd Pointer to data buffer from slave
 * @param bAdjust Use 1st 2 bytes returned as new length (=TRUE)
 * @return I2C_SUCCESS(=0), I2C_BUF_OVRFLO(=0x22)
 */
UINT8 i2c_cmd_WrRd(UINT8 ucCmd, UINT8 ucBytes_wr, UINT8 *ucData_wr, UINT16 ucBytes_rd, UINT8 *ucData_rd, BOOL bAdjust)
{

	/* Sanity Check for Maximum Buffer Size */
	if (ucBytes_wr > BUF_150)
	{
		return I2C_BUF_OVRFLO;
	}

	switch (ucCmd)
	{
		/* Perform a Single Write */
		case WRITE:
			if(HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDR, ucData_wr, ucBytes_wr, I2_TIMEOUT_PERIOD) != HAL_OK)
			{
				error_handler("i2c ", 0, I2C_ERROR);
			}
			break;

		/* Perform a Single Read */
		case READ:

			if(HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDR, ucData_rd, ucBytes_rd, I2_TIMEOUT_PERIOD) != HAL_OK)
			{
				error_handler("i2c ", 0, I2C_ERROR);
			}
			break;

		/* Perform a Write and Read Command */
		case WR_RD:
			if(HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDR, ucData_wr, ucBytes_wr, I2_TIMEOUT_PERIOD) != HAL_OK)
			{
				error_handler("i2c ", 0, I2C_ERROR);
			}

			if(HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDR, ucData_rd, ucBytes_rd, I2_TIMEOUT_PERIOD) != HAL_OK)
			{
				error_handler("i2c ", 0, I2C_ERROR);
			}
			break;
		default:
			error_handler("i2c ", 0, I2C_ERROR);
	}

	return I2C_SUCCESS;
}
