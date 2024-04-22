
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/i2c1_master.h"
#include "mcc_generated_files/examples/i2c1_master_example.h"

#include <string.h>
#define PERIOD 0x80
#define ZERO 0x00

#define I2C1_MASTER_H
#include <stdbool.h>

#define I2C1_MASTER_EXAMPLE_H

#include <stdint.h>
#include <stdio.h>

//Temp Sensor Info
#define TEMP_ADDR 0x4C
#define TEMP_REG 0x0
uint8_t temp_value = 0x0;

//Hum Sensor Info
#define HUM_ADDR 0x44
#define HUM_REG 0xE0
uint8_t hum_value = 0x0;

//Motor Driver
uint8_t ALV;
uint8_t PTM;
uint8_t CTM; 
        
uint8_t FORWARD = 0b11101111;
uint8_t BACK = 0b11101101;
uint8_t STOP = 0b11101000;

int D1 = 2000;
int D2 = 1000;
    
//Timer Info
uint16_t timer_ms = 0;
uint16_t timer_s = 0;

//Timer Callback Function
void timer_callback(void){
    timer_ms++;
    if(timer_ms > 1000){
        timer_s = timer_s + 1;
        timer_ms = timer_ms - 1000;
        
        //every 10 sec the LED eyes change state
        if(timer_s > 10){
            B3_EYE_LED_Toggle();
            B4_EYE_LED_Toggle();
            timer_s = 0;
        }
    }
}

//checks if too hot
void check_test(float temp, float hum){
    if(temp > 29 || hum > 42){
        BUZZ_SetHigh();
        B3_EYE_LED_Toggle();
        B4_EYE_LED_Toggle();
        ChipSelect_SetLow();
        PTM = SPI2_ExchangeByte(FORWARD);
        ChipSelect_SetHigh();
        __delay_ms(D1);
    }
}

//test motor function
void motor_test(void){
    ChipSelect_SetLow();
    PTM = SPI2_ExchangeByte(FORWARD);
    ChipSelect_SetHigh();
    printf("One way Pressed Data: %x\n\r", PTM);
    __delay_ms(D1);

    ChipSelect_SetLow();
    CTM = SPI2_ExchangeByte(STOP);
    ChipSelect_SetHigh();
    printf("Stop Pressed Data: %x\n\r", CTM);
    __delay_ms(D2);

    ChipSelect_SetLow();
    ALV = SPI2_ExchangeByte(BACK);
    ChipSelect_SetHigh();
    printf("Or Another Pressed Data: %x\n\r", ALV);
    __delay_ms(D1);

    ChipSelect_SetLow();
    CTM = SPI2_ExchangeByte(STOP);
    ChipSelect_SetHigh();
    printf("Stop Pressed Data: %x\n\r", CTM);
    __delay_ms(D2);
}

//test temp sensor function
void temp_test(void){
    temp_value = I2C1_Read1ByteRegister(TEMP_ADDR, TEMP_REG);
    float temp_data = (float)temp_value;
    printf("Temperature = %f C\r\n", temp_data);
    check_test(temp_data,0);
}

//test hum sensor function
void hum_test(void){
    hum_value = I2C1_Read1ByteRegister(HUM_ADDR, HUM_REG);
    float hum_data = 100 * (float)hum_value/255;
    printf("Humidity = %f RH\r\n", hum_data);
    check_test(0,hum_data);
}

void main(void)
{
    //Initialize Device
    SYSTEM_Initialize();
    EUSART2_Initialize();

    //Start Timers and Interrupts
    TMR2_Initialize();
    INTERRUPT_Initialize();
    
    //Start I2C Connection
    I2C1_Initialize();
    
    //Start SPI Connection
    SPI2_Initialize();
    SPI2_Open(SPI2_DEFAULT);
            
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    //enable timer interrupts
    TMR2_SetInterruptHandler(timer_callback);
    TMR2_Start();
            
    while (1)
    {
        //turn on eyes
        __delay_ms(1000);
        B3_EYE_LED_SetHigh();
        B4_EYE_LED_SetHigh();
        
        //turn off buzzer
        BUZZ_SetLow();
        
        //turn off motor
        ChipSelect_SetLow();
        CTM = SPI2_ExchangeByte(STOP);
        ChipSelect_SetHigh();
        
        //run sensors
        temp_test();
        hum_test();
    }
}
