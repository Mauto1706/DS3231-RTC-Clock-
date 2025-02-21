#include "sspi_cfg.h"



void SSPI_Init()
{
  PC_DDR_DDR5         = 1;  //CLK 
  PC_CR1_C15          = 1;  
  PC_ODR_ODR5         = 0; 
  
  PC_DDR_DDR6         = 1;  // MOSI
  PC_CR1_C16          = 1;  
  PC_ODR_ODR6         = 0; 
  
  PC_DDR_DDR7         = 0;  //MISO
  PC_CR1_C17          = 1;  
  PC_ODR_ODR7         = 0;

  PE_DDR_DDR5         = 1;  //CS
  PE_CR1_C15          = 1;  
  PE_ODR_ODR5         = 0;  
}