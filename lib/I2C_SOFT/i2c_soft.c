#include "i2c_soft.h"
 
#define I2C_TIME_DELAY      5     // Min is 2us

#define I2C_CYCLE_TIME_OUT  3
#define I2C_COUNT_TIME_OUT  1000

void I2c_Soft_Init()
{
  I2c_SDA_Setup(1);
  I2c_SCL_Setup(1);
  I2x_SDA_ON;
  I2x_SCL_ON;
}

void I2c_Soft_Start()
{
  I2x_SDA_ON;
  I2x_SCL_ON;
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SDA_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SCL_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  
}
void I2c_Soft_Stop()
{
  I2x_SDA_ON;
  I2x_SCL_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SCL_ON;
  I2x_SDA_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SDA_ON;
  I2c_Delay(I2C_TIME_DELAY);
}
void I2c_Soft_WriteByte(uint8_t byte)
{
  uint16_t CountTime = 0;
  for(uint8_t i = 0; i < 8; i++)
  {
    if((byte & 0x80) == 0x80)
    {
      I2x_SDA_ON;
    }
    else 
    {
      I2x_SDA_OFF;
    }
    I2x_SCL_ON;
    I2c_Delay(I2C_TIME_DELAY);
    I2x_SCL_OFF;
    if(i != 7)
    {
      I2c_Delay(I2C_TIME_DELAY);
      byte = byte << 1;
    }
  }
  I2x_SDA_ON;// chac chan tren day sda co logic bang 1
  I2c_SDA_Setup(0);
  I2x_SCL_ON; // bat tat xung clock so 9 de slave biet tra loi
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SCL_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  while(I2x_SDA_READ == 1)
  {
    if(++ CountTime == I2C_COUNT_TIME_OUT)
    {
      CountTime = 0;
      I2c_Soft_Stop();
      return;
    }
    I2c_Delay(I2C_CYCLE_TIME_OUT);
  }     
  I2c_SDA_Setup(1);
}

uint8_t I2c_Soft_ReadByte(uint8_t address, uint8_t add_reg)
{
  uint8_t byte_result = 0x00;
  I2c_Soft_Start();
  I2c_Soft_WriteByte((address << 1) | 0x00);
  I2c_Soft_WriteByte(add_reg);
  
  I2c_Soft_Start();
  I2c_Soft_WriteByte((address << 1) | 0x01);
  I2c_SDA_Setup(0);
  
  for(uint8_t i = 0; i < 8; i++)
  {
    byte_result = byte_result << 1;
    I2x_SCL_ON;
    I2c_Delay(I2C_TIME_DELAY/2 + 1);
    byte_result |= I2x_SDA_READ;
    I2c_Delay(I2C_TIME_DELAY/2 + 1);
    I2x_SCL_OFF;
    I2c_Delay(I2C_TIME_DELAY);
  }
  I2c_SDA_Setup(1);//ack
  I2x_SDA_ON;
  I2x_SCL_ON;
  I2c_Delay(I2C_TIME_DELAY);
  I2x_SCL_OFF;
  I2c_Delay(I2C_TIME_DELAY);
  
  I2c_Soft_Stop();
  return byte_result;
}
void I2c_Soft_ReadData(uint8_t address,uint8_t add_reg, uint8_t *data, uint16_t len)
{
  I2c_Soft_Start();
  I2c_Soft_WriteByte(address << 1 | 0x00);
  I2c_Soft_WriteByte(add_reg);
  
  I2c_Soft_Start();
  I2c_Soft_WriteByte(address << 1 | 0x01);
  
  for(uint16_t count = 0; count < len; count++)
  {
    I2c_SDA_Setup(0);
    for(uint8_t i = 0; i < 8; i++)
    {
      data[count] <<= 1;
      I2x_SCL_ON;
      I2c_Delay(I2C_TIME_DELAY/2 + 1);
      data[count]|= I2x_SDA_READ;
      I2c_Delay(I2C_TIME_DELAY/2 + 1);
      I2x_SCL_OFF;
      I2c_Delay(I2C_TIME_DELAY);
    }
    I2c_SDA_Setup(1);//ack
    if(count == (len - 1))
    {
      
      I2x_SDA_ON;
    }
    else
    {
      I2x_SDA_OFF;
    }
    I2x_SCL_ON;
    I2c_Delay(I2C_TIME_DELAY);
    I2x_SCL_OFF;
    I2c_Delay(I2C_TIME_DELAY);
  }
  I2c_Delay(I2C_TIME_DELAY);
  I2c_Soft_Stop();
}