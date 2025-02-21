#include "i2c_soft_cfg.h"


void I2c_SDA_Setup(uint8_t OutEnable)
{
  PB_DDR_DDR5 = OutEnable;
  PB_CR1_C15 = 1;
}


void I2c_SCL_Setup(uint8_t OutEnable)
{
  PB_DDR_DDR5 = OutEnable;
  PB_CR1_C15 = 1;
}



