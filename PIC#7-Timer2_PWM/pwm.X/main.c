/*
 * File:   main.c
 * Author: koral
 *
 * Created on 28 January 2021, 13:26
 */


#include <xc.h>
#include "fuses.h"

#define tmr2_isr_flag PIR1bits.TMR2IF
#define LED_RED PORTDbits.RD0

int i = 0;
int duty_cycle = 0;

void __interrupt() my_isr() {
    if(tmr2_isr_flag) {
        i++;
        /*
         * Interrupt is occured every 512us
         * 512us * 195 = 99.84ms
         * 512us * 390 = 199.6ms
         * This is basically led blink example 
         * with Timer2
         */
        if(i == 195) {
            LED_RED = 0;
        }
        if(i == 390) {
            LED_RED = 1;
            i = 0;
        }
        PR2 = 255;
        tmr2_isr_flag = 0;
    }
}

void main(void) {
    /*
     *Timer2 interrupt flag period -> T_int
     * T_int = T_cycle * Prescaler * (PR2 + 1) * Postscaler
     * T_cylce = 1/(F_osc/4) = 1/(20 MHz / 4) = 0.2us
     * T_int = 0.2us * 1 * (255 + 1) * 10 = 512us = 0.512ms
     */
    //Timer2 Prescalar 1:1
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 0;          //Prescaler is 4
    //Timer2 Postscaler 1:10 -> This for Timer2 Interrupt 
    T2CONbits.TOUTPS3 = 1;
    T2CONbits.TOUTPS2 = 0;
    T2CONbits.TOUTPS1 = 1;
    T2CONbits.TOUTPS0 = 0;
    
    T2CONbits.TMR2ON = 1;           //Timer2 is enable
    
    INTCONbits.GIE = 1;             //Enables all activated interrupts
    INTCONbits.PEIE = 1;            //Enable peripheral interrupt sources
    PIE1bits.TMR2IE = 1;            //Timer2 interrupt is enabled
    //PWM Mode for CCP1 Module
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
    CCP1CONbits.CCP1M1 = 1;
    CCP1CONbits.CCP1M0 = 1;
    //PWM Mode for CCP2 Module
    CCP2CONbits.CCP2M3 = 1;
    CCP2CONbits.CCP2M2 = 1;
    CCP2CONbits.CCP2M1 = 1;
    CCP2CONbits.CCP2M0 = 1;

    /*
     * In the datasheet there are some specific value 
     * for generating pwm signal with particular frequency
     * Prescaler = 1
     * PR2 = 255
     * Then, we have 10-bit pwm resolution with 19.53 kHz
     */
    PR2 = 255;
    
    TRISC = 0xF9;                       //For PWM, TRISC<2,1> must be cleared
    
    TRISD = 0xFE;                       //RD0 is output
    PORTD = 0x00;
    
    while(1) {
        for(duty_cycle = 0 ; duty_cycle < 1024 ; duty_cycle++) {
            CCPR1L = duty_cycle >> 2;
            CCP1CONbits.CCP1X = duty_cycle & 0x002;
            CCP1CONbits.CCP1Y = duty_cycle & 0x001;
            
            CCPR2L = (1023 - duty_cycle) >> 2;
            CCP2CONbits.CCP2X = (1023 - duty_cycle) & 0x002;
            CCP2CONbits.CCP2Y = (1023 - duty_cycle) & 0x001;
            __delay_ms(1);
        }
        for(duty_cycle = 1023 ; duty_cycle >= 0 ; duty_cycle--) {
            CCPR1L = duty_cycle >> 2;
            CCP1CONbits.CCP1X = duty_cycle & 0x002;
            CCP1CONbits.CCP1Y = duty_cycle & 0x001;
            
            CCPR2L = (1023 - duty_cycle) >> 2;
            CCP2CONbits.CCP2X = (1023 - duty_cycle) & 0x002;
            CCP2CONbits.CCP2Y = (1023 - duty_cycle) & 0x001;
            __delay_ms(1);
        }
    }
}
