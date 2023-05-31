#ifndef I2CIO_H
#define	I2CIO_H

#ifdef	__cplusplus
extern "C" {
#endif


/* DEFINES */
#define CONFIG_I2C_VAL  0x8000
#define CONFIG_BRG_VAL  8
#define WRITE           1
#define READ            2
#define WR_RD           3
#define SLAVE_ADDR      0x80

/* FUNCTION DECLARATIONS */
UINT8 i2c_cmd_WrRd(UINT8, UINT8,  UINT8 *, UINT16,  UINT8 *, BOOL);



#ifdef	__cplusplus
}
#endif

#endif	/* I2CIO_H */

