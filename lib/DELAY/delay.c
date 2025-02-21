#include "delay.h"

void delay_us(unsigned int time)
{
  while(time--)
  {
    asm("nop");              // mat 2 chu ky may
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
  }
}

void delay_ms(unsigned int time)
{
  while(time--)
    delay_us(1000);
}
