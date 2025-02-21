#include "clock.h"

void Init_ClockCPU(void)
{
  CLK_ICKR_LSIEN     = 1;
  CLK_ECKR_HSEEN     = 0;
  CLK_CKDIVR_HSIDIV  = 0x00;   
  CLK_SWCR_SWEN      = 1;
  CLK_SWR            = 0xE1; 
}