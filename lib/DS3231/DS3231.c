#include "DS3231.h"

#define DS3231_add      0x68
uint8_t BcdToDec(uint8_t Bcd)
{
  return(Bcd & 0x0F) + ((Bcd >> 4) & 0x0F)*10;
}
 

uint8_t DecToBcd(uint8_t Dec)
{
  return (Dec%10) | ((Dec/10) << 4);
}

void WriteReg_DS3231(uint8_t add_reg, uint8_t byte)
{
  I2c_Soft_Start();
  I2c_Soft_WriteByte(DS3231_add << 1 | 0x00);
  I2c_Soft_WriteByte(add_reg);
  I2c_Soft_WriteByte(byte);
  I2c_Soft_Stop();
}

void WriteTime_DS3231(uint8_t gio, uint8_t phut, uint8_t giay)
{
  WriteReg_DS3231(0x00, DecToBcd(giay));
  WriteReg_DS3231(0x01, DecToBcd(phut));
  WriteReg_DS3231(0x02, DecToBcd(gio) & 0x3F);
}

void WriteDate_DS3231(uint8_t thu, uint8_t ngay, uint8_t thang, uint8_t nam)
{
  WriteReg_DS3231(0x03, DecToBcd(thu));
  WriteReg_DS3231(0x04, DecToBcd(ngay));
  WriteReg_DS3231(0x05, DecToBcd(thang));
  WriteReg_DS3231(0x06, DecToBcd(nam));
}

void ReadTime_DS3231(uint8_t *gio, uint8_t *phut, uint8_t *giay)
{
  *giay = BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x00) & 0x7F);
  *phut = BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x01) & 0x7F);
  *gio  =  BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x02) & 0x3F);
}

void ReadDate_DS3231(uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam)
{
  *thu = BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x03) & 0x07);
  *ngay = BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x04) & 0x3F);
  *thang = BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x05) & 0x1F);
  *nam  =  BcdToDec(I2c_Soft_ReadByte(DS3231_add, 0x06));
} 