#include "i2c_soft.h"
#include "std_type.h"

uint8_t BcdToDec(uint8_t Bcd); 
uint8_t DecToBcd(uint8_t Dec);
void WriteTime_DS3231 (uint8_t gio, uint8_t phut, uint8_t giay);
void WriteDate_DS3231 (uint8_t thu,uint8_t ngay, uint8_t thang, uint8_t nam);
void ReadTime_DS3231 (uint8_t *gio, uint8_t *phut, uint8_t *giay);
void ReadDate_DS3231(uint8_t *thu, uint8_t *ngay, uint8_t *thang, uint8_t *nam);