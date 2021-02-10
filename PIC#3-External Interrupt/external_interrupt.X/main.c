/*
 * File:   main.c
 * Author: koral
 *
 * Created on 26 January 2021, 23:54
 */


#include <xc.h>
#include "fuses.h"

#define interrupt_flag INTCONbits.INTF      //External interrupt flag

#define LED_RED PORTDbits.RD0
#define LED_YELLOW PORTDbits.RD1
#define Button PORTDbits.RD2

void __interrupt() my_isr_routine (void) {
    if(interrupt_flag) {						//Interrupt has been triggered 
        LED_RED = 1;			
        interrupt_flag = 0;                     //Clear interrupt flag
    }
}

void main(void) {
    OPTION_REGbits.nRBPU = 0;   //Internal Pull-up resistors are enabled
    OPTION_REGbits.INTEDG = 1;  //Interrupt n rising edge
    INTCONbits.INTE = 1;        //Enable external interrupt source
    INTCONbits.GIE = 1;         //Enable interrupts
    
    TRISD = 0xFC;               //0b11111110 -> RD0 and RD1 are output
    PORTD = 0x00;               //All ports in PORTD are clear
    
    while(1) {
        if(Button) {
            __delay_ms(100);
            LED_RED = 1;
        }
        else {
            LED_RED = 0;
        }
        LED_YELLOW = 1;
        __delay_ms(2000);
        LED_YELLOW = 0;
        __delay_ms(1000);
    }
}
