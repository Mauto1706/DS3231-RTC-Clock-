#include "sspi.h"

#define LOW     0
#define HIGH    1

uint8_t Sspi_Transmit(uint8_t byte_s)
{
  uint8_t byte_r = 0x00;
  //SSPI_NSS = LOW;
  
  for(uint8_t i = 0; i < 8; i++)
  {
    SSPI_MOSI = (byte_s >>(7 - i)) & 0x01;
    SSPI_SCK = HIGH;
    //delay_ms(1);
    SSPI_SCK = LOW;
    byte_r |= SSPI_MISO;
    if(i < 7)
    {
      byte_r << 1;
    }
    //delay_ms(1);
  }
  //SSPI_NSS = HIGH;
  
  return byte_r;
}
