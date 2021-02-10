/*
 * File:   main.c
 * Author: koral
 *
 * Created on 25 January 2021, 20:10
 */
#include <xc.h>
#include "fuses.h"

#define LED PORTDbits.RD0

void main(void) {
    TRISD = 0xFE;   //0b11111110 -> RD0 is output
    PORTD = 0x00;   //All ports in PORTD are clear
    
    while(1) {
        LED = 1;
        __delay_ms(100);
        LED = 0;
        __delay_ms(500);
    }
}
