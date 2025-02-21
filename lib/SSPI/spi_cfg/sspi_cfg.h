#include "iostm8s005k6.h"



#define SSPI_MOSI       PC_ODR_ODR6
#define SSPI_SCK        PC_ODR_ODR5
#define SSPI_NSS        PE_ODR_ODR5
#define SSPI_MISO       PC_IDR_IDR7

void SSPI_Init(void);