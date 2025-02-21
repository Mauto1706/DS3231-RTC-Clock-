#include "gpio.h"


void Init_GPIO(void)
{
  /* IC_STM8S_LED1 */
  PB_DDR_DDR0        = 1;  // output
  PB_CR1_C10         = 1;  // pull-up
  PB_ODR_ODR0        = 0;  // out 0V
  
  PB_DDR_DDR1        = 1;  // output
  PB_CR1_C11         = 1;  // pull-up
  PB_ODR_ODR1        = 0;  // out 0V
  
  PB_DDR_DDR2        = 1;  // output
  PB_CR1_C12         = 1;  // pull-up
  PB_ODR_ODR2        = 0;  // out 0V
  
  PB_DDR_DDR3        = 1;  // output
  PB_CR1_C13         = 1;  // pull-up
  PB_ODR_ODR3        = 0;  // out 0V
  
  PC_DDR_DDR1        = 1;  // output
  PC_CR1_C11         = 1;  // pull-up 
  PC_ODR_ODR1        = 0;  // out 0V
  
  PC_DDR_DDR2        = 1;  // output
  PC_CR1_C12         = 1;  // pull-up
  PC_ODR_ODR2        = 0;  // out 0V
    
  PC_DDR_DDR3        = 1;  // output
  PC_CR1_C13         = 1;  // pull-up
  PC_ODR_ODR3        = 0;  // out 0V
  
  PC_DDR_DDR4        = 1;  // output
  PC_CR1_C14         = 1;  // pull-up
  PC_ODR_ODR4        = 0;  // out 0V
  
  PD_DDR_DDR0        = 0;
  PD_CR1_C10         = 0;  // 
  PD_ODR_ODR0        = 0;  // out 0V
  
  PD_DDR_DDR2        = 0;
  PD_CR1_C12         = 0;  
  PD_ODR_ODR2        = 0;  // out 0V
  
  PD_DDR_DDR3        = 0;
  PD_CR1_C13         = 0;  
  PD_ODR_ODR3        = 0;  // out 0V
  
  PD_DDR_DDR4        = 0;
  PD_CR1_C14         = 0;  
  PD_ODR_ODR4        = 0;  // out 0V
}