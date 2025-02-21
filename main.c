#include "iostm8s005k6.h"
#include <intrinsics.h>
#include "delay.h"
#include "DS3231.h"
#include "i2c_soft.h"
#include "gpio.h"
#include "clock.h"
#include "sspi.h"
#include "std_type.h"



#define HC595_LATCH       PE_ODR_ODR5

#define STD_ON         1
#define STD_OFF        0

#define LED0           PB_ODR_ODR0
#define LED1           PB_ODR_ODR1
#define LED2           PB_ODR_ODR2
#define LED3           PB_ODR_ODR3
#define LED4           PC_ODR_ODR1
#define LED5           PC_ODR_ODR2
#define LED6           PC_ODR_ODR3
#define LED7           PC_ODR_ODR4

#define MODE_RUNNING    0
#define MODE_MENU       1
#define MODE_SET_TIME   2
#define MODE_SET_DATE   3
#define MODE_SET_ALAM   4
typedef void (* StateFunc)(void);

void Des_showDateTime(void);
void Des_showMenu(void);
void Des_setTime(void);

const uint8_t ledCode[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
const uint8_t scanCode[4] = {0x0E, 0x0D, 0x0B, 0x07};
uint8_t value_led7seg [4] = {0x00, 0x00, 0x00, 0x00};
uint8_t *led7_seg;
uint16_t key_status;
uint8_t led_dot = 0x00;
uint8_t menu[4][4] = {
                                {0x78, 0x04, 0x54, 0x79}, // time
                                {0x5E, 0x77, 0x78, 0x79}, // date
                                {0x77, 0x06, 0x77, 0x54}, // alam
                                {0x00, 0x5C, 0x1C, 0x78}  // exit
                             };
const uint16_t keyMask[4] = {0xEEEE, 0xDDDD, 0xBBBB, 0x7777};
uint8_t button_status[16];


uint8_t state_program = MODE_RUNNING;



StateFunc func_list[4] = {  Des_showDateTime,
                            Des_showMenu,
                            Des_setTime,
                            Des_showMenu
                         };



static void Init_Time2(void)
{
    TIM2_PSCR          = 5;
    TIM2_ARRH          = (uint8_t)((1000 >> 8) & 0xFF);
    TIM2_ARRL          = (uint8_t)(1000 & 0xFF);
    TIM2_CR1_ARPE      = 1;
    TIM2_IER_UIE       = 1;
    TIM2_CR1_CEN       = 1;
}/*Ket Thuc Init_Time2*/

typedef union
{
  struct
  {
    uint8_t PIN0 : 1;
    uint8_t PIN1 : 1;
    uint8_t PIN2 : 1;
    uint8_t PIN3 : 1;
    uint8_t PIN4 : 1;
    uint8_t PIN5 : 1;
    uint8_t PIN6 : 1;
    uint8_t PIN7 : 1;
  }PIN;
  uint8_t ODR;
}HC595_RegType;

HC595_RegType  REG1;

void HC595_Init()
{
  SSPI_Init();
}

void HC595_SendByte(uint8_t byte)
{
  Sspi_Transmit(byte);
  HC595_LATCH = HIGH;
  HC595_LATCH = LOW;
}

typedef struct
{
  uint8_t data[20];
  uint8_t length;
}Data;

void update_value7seg(uint16_t value, uint8_t opt, uint8_t eff)
{
  uint8_t nghin, tram, chuc, donvi;
  
  static uint16_t countTime = 0;
  
  nghin = value / 1000;
  tram = (value / 100) % 10;
  chuc = (value / 10) % 10;
  donvi = value % 10;
  if(countTime < 400)
  {
    if(opt == 0)
    {
      value_led7seg[0] = ledCode[nghin];
      value_led7seg[1] = (led_dot & 0x02) ? (ledCode[tram] | 0x80) : ledCode[tram];
      value_led7seg[2] = ledCode[chuc];
      value_led7seg[3] = (led_dot & 0x08) ? (ledCode[donvi] | 0x80) : ledCode[donvi];
    }
    else
    {
      value_led7seg[0] = (led_dot & 0x01) ? (ledCode[nghin] | 0x80) : ledCode[nghin];
      value_led7seg[1] = (led_dot & 0x02) ? (ledCode[tram] | 0x80) : ledCode[tram];      
      value_led7seg[2] = (led_dot & 0x04) ? (ledCode[chuc] | 0x80) : ledCode[chuc];
      value_led7seg[3] = (led_dot & 0x08) ? (ledCode[donvi] | 0x80) : ledCode[donvi];
    }
  }
  else
  {
    value_led7seg[0] = 0x00;
    value_led7seg[1] = 0x00;
    value_led7seg[2] = 0x00;
    value_led7seg[3] = 0x00;
  }   
  if(eff == 0)
  {
     countTime = 0;
  }
  else
  {
    if(++ countTime == 800)
    {
      countTime = 0;
    }
  }
}

void Des_showDateTime(void)
{
  uint8_t gio = 0, phut = 0, giay = 0, thu = 0, ngay = 0, thang = 0, nam = 0;
  uint8_t countDot = 0;
  uint8_t countEffect = 0;

  button_status[0] = 0;
  button_status[1] = 0;
  button_status[2] = 0;
  led7_seg = value_led7seg;
  while(state_program == MODE_RUNNING)
  {
   if(button_status[0] == 2)
   {
     state_program = MODE_MENU;
     break;
   }
   if(++ countEffect == 70)
   {
     countEffect = 0;
   }
   else if(countEffect < 20)
   {
     led_dot = 0x02;
     update_value7seg(ngay * 100 + thang, 1, 0); 
   }
   else
   {
     if(++ countDot == 10)
     {
       led_dot |= 0x02;
       countDot = 0;
     }
     else if(countDot == 5)
     {
       led_dot &= ~0x02;
     }
     update_value7seg(gio * 100 + phut, 1, 0);
   }
   ReadTime_DS3231(&gio, &phut, &giay);
   ReadDate_DS3231(&thu, &ngay, &thang, &nam);
   delay_ms(100);
 }
}

void Des_showMenu(void)
{
  int8_t count = 0;
  
  button_status[0] = 0;
  button_status[1] = 0;
  button_status[2] = 0;
  
  
  while(state_program == MODE_MENU)
  {
    if(button_status[0] = 1)// up
    {
      if(++ count > 3)
      {
        count = 0;
      }
    }
    else if(button_status[1] = 1)// down
    {
      if(--count < 0)
      {
        count = 3;
      }
    }
    led7_seg = (uint8_t*)&menu[count][0];
    delay_ms(100);  
    if(button_status[2] == 1)// chon
    {
      if(count == 0)
      {
        state_program = MODE_SET_TIME;
        break;
      }
      else if(count == 1)
      {
        state_program = MODE_SET_DATE;
        break;
      }
      else if(count == 2)
      {
        state_program = MODE_SET_ALAM;
        break;
      }
      else 
      {
        state_program = MODE_RUNNING;
        break;
      }
    }
  }
}

void Des_setTime(void)
{
  uint8_t gio = 0, phut = 0, giay = 0;
  
  button_status[0] = 0;
  button_status[1] = 0;
  button_status[2] = 0; 
  button_status[3] = 0; 
  
  led_dot &= ~0x02;
  
  ReadTime_DS3231(&gio, &phut, &giay);
  update_value7seg(gio * 100 + phut, 0, 1);
  int8_t mode = 0;
  while(state_program == MODE_SET_TIME)
  {
    if(button_status[3] == 1)
    { 
      WriteTime_DS3231(gio, phut, giay);
      state_program = MODE_MENU;
      break;
    }
    if(button_status[0] == 1)
    {
      if(++ mode == 2)
      {
        mode = 0;
      }
    }
    if (mode == 0)// chinh gio
    {
      led_dot = led_dot & (~0x08) | 0x02;
      if(button_status[1] == 1)
      {
        if(++gio > 23)
        {
          gio = 0;
        }
      }
      if(button_status[2] == 1)
      {
        if(gio == 0)
        {
          gio = 23;
        }
        else
        {
          gio--;
        }
      }
    }
    else// chinh phut
    {
      led_dot = led_dot & (~0x02) | 0x08;
      if(button_status[1] == 1)
      {
        if(++phut > 59)
        {
          phut = 0;
        }
      }
      if(button_status[2] == 1)
      {
        if(phut == 0)
        {
          phut = 59;
        }
        else
        {
          phut--;
        }
      }
    }
    update_value7seg(gio * 100 + phut, 0, 1);
    delay_ms(1);
  }
}

int main()
{  
  __disable_interrupt();
  
  Init_ClockCPU();
  Init_GPIO();
  I2c_Soft_Init();
  Init_Time2();
  HC595_Init();
  led7_seg = value_led7seg;
  
  __enable_interrupt();
  while(1)
  {
    func_list[state_program](); 
  }
}

#pragma vector = TIM2_OVR_UIF_vector
__interrupt void TIM2_OVR_UIF_handler(void)
{
  static uint8_t count = 0;
         uint8_t byteinPD = 0x00;
  static uint16_t countTime_button[16];       
     
  HC595_SendByte(scanCode[count]);
  
  
  PB_ODR = (PB_ODR & 0xF0) | (led7_seg[count] & 0x0F);     // quet led
  PC_ODR = (PB_ODR & 0xE1) | ((led7_seg[count] >> 3) & 0x1E);
  
  byteinPD = PD_IDR;// cap nhat keystatus
  byteinPD = byteinPD & 0x01 | (PD_IDR >> 1) & 0x0F;
  key_status = (key_status & keyMask[count]) | (uint16_t)(byteinPD << (count * 4));
  
   if((key_status & 0x01) == 0x01)
  {
    countTime_button[0]++;
    if(countTime_button[0] == 5)//check chong doi
    {
      button_status[0] = 1; // an nha
    }
    else if(countTime_button[0] >= 500)
    {
       countTime_button[0] = 450;
       button_status[0] = 2;// an giu
    }
  }
  else 
  {
    countTime_button[0] = 0;
  }   
  if((key_status & 0x10) == 0x10)
  {
    countTime_button[1]++;
    if(countTime_button[1] == 5)//check chong doi
    {
      button_status[1] = 1; // an nha
    }
    else if(countTime_button[1] >= 500)
    {
       countTime_button[1] = 450;
       button_status[1] = 2;// an giu
    }
  }
  else 
  {
    countTime_button[1] = 0;
  }
  if((key_status & 0x04) == 0x04)
  {
    countTime_button[2]++;
    if(countTime_button[2] == 5)//check chong doi
    {
      button_status[2] = 1; // an nha
    }
    else if(countTime_button[2] >= 500)
    {
       countTime_button[2] = 450;
       button_status[2] = 2;// an giu
    }
  }
  else 
  {
    countTime_button[2] = 0;
  }
  
  
    if(++count > 3)
  {
    count = 0;
  }
  TIM2_SR1_UIF = 0;
}
