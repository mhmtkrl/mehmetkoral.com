/*
 * File:   main.c
 * Author: koral
 *
 * Created on 26 January 2021, 23:10
 */


#include <xc.h>
#include "fuses.h"

#define Button PORTDbits.RD2
#define LED    PORTDbits.RD0

void main(void) {
    TRISD = 0xFE;   //0b11111110
    PORTD = 0x00;
    
    while(1) {
        if(Button) {
            LED = 1;
            while(Button);
        }
        LED = 0;
    }
}
