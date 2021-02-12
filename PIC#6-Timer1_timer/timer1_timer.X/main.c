/*
 * File:   main.c
 * Author: koral
 *
 * Created on 28 January 2021, 01:44
 */


#include <xc.h>
#include "fuses.h"

#define interrupt_flag PIR1bits.TMR1IF
#define LED_RED PORTDbits.RD0
#define LED_YELLOW PORTDbits.RD1

int i = 0;

void __interrupt() _isr() {
    if(interrupt_flag) {
        i++;
        if(i == 1) {
            LED_RED = 0;
        }
        if(i == 2) {
            LED_RED = 1;
            i = 0;
        }
        TMR1H=0x0B; 
        TMR1L=0xDC;
        interrupt_flag = 0;
    }
}

void main(void) {
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;      //1:8
    T1CONbits.T1OSCEN = 1;      //Oscillator is enabled
    T1CONbits.T1INSYNC = 1;     //Do not synchronize external clock input
    INTCONbits.PEIE = 1;
    T1CONbits.TMR1CS = 0;       //Internal clock
    T1CONbits.TMR1ON = 1;       //Timer1 is enable
    PIE1bits.TMR1IE = 1;        //Timer1 interrupt sequence is enanbled         
    INTCONbits.GIE = 1;         //Enables all interrupts sources
    TMR1H=0x0B; 
    TMR1L=0xDC;
    /*
    (TMR1H << 8 + TMR1L) = 65536 - (T_interrupt/(T_cyle * Prescaler))
     * T_interrupt = 1000 us (we want interrupt every 1ms)
     * Tcyle = 1/F_osc/4 -> 1/20000000/4 = 0.2us
     * Prescaler = 8
     * (TMR1H << 8 + TMR1L) = 65536 - (100000/(0.2*32)) = 3036(Decimal) = 0x0BDC
     * The interrupt will be happened every 100ms
    */
    
    TRISD = 0x00;
    PORTD = 0x00;
    
    while(1) {
        LED_YELLOW = 1;
        __delay_ms(100);
        LED_YELLOW = 0;
        __delay_ms(100);
    }
}
