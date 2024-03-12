#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"

#include <stdint.h>
#include <stdio.h>

//humidity sensor info
#define HTU31_I2C_ADDR 0x40
#define HTU31_REG 0x0D


void main(void)
{
    SYSTEM_Initialize();
    EUSART2_Initialize();
    I2C1_Initialize();

    uint8_t hum_data;

    while (1)
    {
        i2c1_error_t result = I2C1_Open(HTU31_I2C_ADDR);
        
        __delay_ms(10);

        if (result == I2C1_NOERR)
        {
            printf("Success! \n\r");
            __delay_ms(100);
            
            hum_data = I2C1_Read1ByteRegister(HTU31_I2C_ADDR, HTU31_REG);
            
            int hum = (float)data * 100 / 255;
        
            printf("Data Value: %u\n\r", hum);
        }
        else if (result == I2C1_BUSY)
        {
            printf("I2C Busy");
            __delay_ms(100);
        }
        else
        {
            printf("Error!");
            __delay_ms(100);
        }
    }
}
