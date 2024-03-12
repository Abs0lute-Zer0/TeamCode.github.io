#include "mcc_generated_files/mcc.h"

#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"

#include <string.h>
# define PERIOD 0x80
# define ZERO 0x00
# define I2C_ADDR_TC74      0x4C // TC74 Temp Sensor A5 Adress
#define I2C1_MASTER_H
#include <stdbool.h>

#define I2C1_MASTER_EXAMPLE_H

#include <stdint.h>
#include <stdio.h>




uint8_t Asshole= 0x0;

uint16_t timer_ms= 0;
uint16_t timer_s= 0; 
float time = 0;
float TimeK=0;
int Bitch=100;


void timer_callback(void){
   
    timer_ms++;
            
    if (timer_ms > 1000){
        timer_s= timer_s+1;
        timer_ms=timer_ms-1000;
      
        
    }
    
}


/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    EUSART2_Initialize();
    TMR2_Initialize();
    I2C1_Initialize();
    INTERRUPT_Initialize ();




    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
    TMR2_SetInterruptHandler(timer_callback);
    TMR2_Start();

    while (1)
    {
        timer_callback();
       
        time = (float)timer_s + (((float)timer_ms)/1000);
        printf("t= %0.2f s\n\r", time);
        
       
       Asshole = I2C1_Read1ByteRegister(I2C_ADDR_TC74, 0x0 );
       printf("Temperature = %u c\r\n", Asshole);

        
    }
}
