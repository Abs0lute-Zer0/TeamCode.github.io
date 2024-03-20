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
            
            int hum = (float)hum_data * 100 / 255;
        
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


#define HTU31D_ADDR 0x40
#define HTU31D_REG 0x08

void main(void)
{
    SYSTEM_Initialize();
    EUSART2_Initialize();
    I2C1_Initialize(); 
    
    __delay_ms(10);
    
    uint8_t data;
    
    LED_SetHigh();
    
    while (1)
    {
        I2C1_Open(HTU31D_ADDR);
        
        __delay_ms(10);
        
        //I2C1_ReadNBytes(i2c1_address_t 0x40, uint8_t 0x08, size_t 1);
        while(!I2C1_Open(0x40)); // sit here until we get the bus..
        I2C1_SetBuffer(0x08,1);
        I2C1_SetAddressNackCallback(NULL,NULL); //NACK polling?
        I2C1_MasterWrite();
        while(I2C1_BUSY == I2C1_Close()); // sit here until finished.

        __delay_ms(10);
        
        while(!I2C1_Open(0x40)); // sit here until we get the bus..
        I2C1_SetBuffer(0x08,1);
        data = I2C1_MasterRead();
        while(I2C1_BUSY == I2C1_Close()); // sit here until finished.
                       
        //float hum = (float)data * 100 /(255);
        __delay_ms(10);

        printf("Data Value: %u\n\r", data);
        
        __delay_ms(10);

    }
}
