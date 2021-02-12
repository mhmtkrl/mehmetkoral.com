/*
 * File:   main.c
 * Author: koral
 *
 * Created on 28 January 2021, 00:16
 */


#include <xc.h>
#include "fuses.h"

#define interrupt_flag INTCONbits.TMR0IF

int i = 0;

void __interrupt() my_isr() {
    if(interrupt_flag) {
        i++;
        TMR0 = 255;
        interrupt_flag = 0;
    }
}

void main(void) {
    ADCON1 |= 0x07;             //PORTA is used for digital purposes
    TRISA = 0xFF;               //All pins are in the PORTA are set as input
    OPTION_REGbits.T0CS = 1;    //External clock cource (RA4) for Timer0
    OPTION_REGbits.T0SE = 1;    //Falling edge triggering
    OPTION_REGbits.PSA = 1;     //Prescaler for WDT! In this way Prescaler is 1:1
    INTCONbits.TMR0IE = 1;      //Timer0 interrupt service is enabled
    INTCONbits.GIE = 1;         //Enables all interrupts sources
    TMR0 = 255;
    
    TRISD = 0x00;
    PORTD = 0x00;
    
    while(1) {
        PORTD = i;
    }
}
