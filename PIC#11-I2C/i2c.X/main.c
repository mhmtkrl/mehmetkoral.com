/*
 * File:   main.c
 * Author: koral
 *
 * Created on 31 January 2021, 13:01
 */


#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "fuses.h"
#include "uart.h"
#include "i2c.h"

uint8_t tc74_ready;
int8_t temp_value = 0;
char txt[] = "\r\nPIC#9-I2C Communication\r\n";

void main(void) {
    //Initialization of peripherals
    TRISD = 0xFC;
    PORTD = 0x00;
    UART_Init();                //Baud rate = 57600 baud/s
    UART_Transmit_String(txt);
    i2c_init();                   //100 kHz I2C Mode
    ///////////////////////////////////////////////////
    //Wait for the sensor value is readable
    __delay_ms(1000);
    tc74_ready = tc74_temp_ready();
    
    if(tc74_ready == 0x40) PORTD = 0x01;
    else                   PORTD = 0x00;
    
    while(1) {
        temp_value = tc74_read_temp();
        sprintf(txt, "Temp Value = %d\r\n", temp_value);
        UART_Transmit_String(txt);
        __delay_ms(500);
    }
}

