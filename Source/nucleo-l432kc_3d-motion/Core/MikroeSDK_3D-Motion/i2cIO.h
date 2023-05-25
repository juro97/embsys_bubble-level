#ifndef I2CIO_H
#define	I2CIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CONFIG_I2C_VAL  0x8000
#define CONFIG_BRG_VAL  8
#define WRITE           1
#define READ            2
#define WR_RD           3
#define SLAVE_ADDR      0x80

#include "main.h"

void gets_I2C1(UINT8 *, UINT16, BOOL);
HAL_StatusTypeDef i2c_cmd_WrRd(uint8_t, uint8_t,  uint8_t *, uint16_t,  uint8_t *, BOOL);

#ifdef	__cplusplus
}
#endif

#endif	/* I2CIO_H */

