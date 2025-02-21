#include "delay.h"
#include "std_type.h"
#include "iostm8s005k6.h"


#define I2c_Delay(us)        delay_us(us)

#define I2x_SDA_ON           PB_ODR_ODR5 = 1
#define I2x_SDA_OFF          PB_ODR_ODR5 = 0
#define I2x_SDA_READ         PB_ODR_ODR5

#define I2x_SCL_ON           PB_ODR_ODR4 = 1
#define I2x_SCL_OFF          PB_ODR_ODR4 = 0


void I2c_SDA_Setup(uint8_t OutEnable);
void I2c_SCL_Setup(uint8_t OutEnable);