/*
 * File:   main.c
 * Author: koral
 *
 * Created on 27 January 2021, 20:12
 */


#include <xc.h>
#include "fuses.h"

#define interrupt_flag INTCONbits.T0IF
#define LED_RED PORTDbits.RD0
#define LED_YELLOW PORTDbits.RD1

int count = 0;

void __interrupt() my_isr() {
    if(interrupt_flag) {
        /*
         * interrupt is occured every 1ms
         * After 100 interrupt sequence we get 100ms
         * After 200 interrupt sequence we get 200ms
         */
        count++;               
        if(count == 100) {
            LED_YELLOW = 0;     //Logic-0 during 100ms
        }
        if(count == 201) {      
            LED_YELLOW = 1;     //Logic-1 during 100ms
            count = 0;
        }
        TMR0 = 100;
        interrupt_flag = 0;
    }
}

void main(void) {
    OPTION_REGbits.T0CS = 0;    //Internal clock source -> Timer Mode
    OPTION_REGbits.PSA = 0;     //Prescaler for Timer0
    OPTION_REGbits.PS2 = 1;     //PS2 = 1
    OPTION_REGbits.PS1 = 0;     //PS1 = 0
    OPTION_REGbits.PS0 = 0;     //PS0 = 0
    INTCONbits.TMR0IE = 1;      //TMR0 interrupt is enabled
    //INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;         //Enables all interrupts sources
    TMR0 = 100;
    
    /*
    TMR0 = 256 - (T_interrupt/(T_cyle * Prescaler))
     * T_interrupt = 1000 us (we want interrupt every 1ms)
     * Tcyle = 1/F_osc/4 -> 1/20000000/4 = 0.2us
     * Prescaler = 32
     * TMR0 = 256 - (1000/(0.2*32)) = 99.75 ~ 100
    */
    
    TRISD = 0xFC;
    PORTD = 0x00;
    
    while(1) {
        LED_RED = 1;
        __delay_ms(100);
        LED_RED = 0;
        __delay_ms(100);
    }
}
