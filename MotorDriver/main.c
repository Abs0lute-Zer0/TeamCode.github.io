#include "mcc_generated_files/mcc.h"
#include <stdio.h>
#include <string.h>
    
//uint8_t dataBytes[] [0b11101111 0b11101101 0b11101100];

uint8_t ALV;
uint8_t PTM;
uint8_t CTM;

uint8_t ALV1 = 0b11101111;
uint8_t PTM1 = 0b11101101;
uint8_t CTM1 = 0b11101000;
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    SPI1_Initialize();
    EUSART2_Initialize();
    
    
    volatile uint8_t rxData;
    
    SPI1_Open(SPI1_DEFAULT);
    
    while (1)
    {
        if(Switch_GetValue()==0)
        {
          __delay_ms(50);
       ChipSelect_SetLow();
       PTM = SPI1_ExchangeByte(PTM1);
       ChipSelect_SetHigh();
       LED_SetHigh();
       __delay_ms(50);
       printf("Pressed Data: %x\n\r", PTM); 
       LED_SetLow();
        }else
        {
           __delay_ms(50);
       ChipSelect_SetLow();
       ALV = SPI1_ExchangeByte(ALV1);
       ChipSelect_SetHigh();
       __delay_ms(50);
       LED_SetLow();
       printf("NotPressed Data: %x\n\r", ALV);
        }
    }     
}
/**
 End of File
*/
