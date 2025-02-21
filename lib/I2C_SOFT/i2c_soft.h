#include "i2c_soft_cfg.h"
#include "std_type.h"

void I2c_Soft_Init(void);

void I2c_Soft_Start(void);
void I2c_Soft_Stop(void);
void I2c_Soft_WriteByte(uint8_t byte);

uint8_t I2c_Soft_ReadByte(uint8_t address, uint8_t add_reg);
void I2c_Soft_ReadData(uint8_t address,uint8_t add_reg, uint8_t *data, uint16_t len);