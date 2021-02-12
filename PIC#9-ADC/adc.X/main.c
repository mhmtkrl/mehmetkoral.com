/*
 * File:   main.c
 * Author: koral
 *
 * Created on 29 January 2021, 12:31
 */


#include <xc.h>
#include <stdio.h>
#include "fuses.h"

#define pot_pin PORTAbits.RA0       //AN0

int adc_data = 0;
unsigned char txt[] = "\r\nPIC#9-A/D Converter\r\n";

void main(void) {
    TRISD = 0x00;
    PORTD = 0x00;
    /////////////////A/D Module////////////////////////
    TRISA = 0xFF;
    ADCON0bits.ADCS1 = 1;
    ADCON0bits.ADCS0 = 0;           //Fosc/64
    //Analog channel is AN0
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;            
    ADCON0bits.CHS0 = 0;
    ADCON1bits.ADFM = 1;            //Right justified result
    ADCON1bits.ADCS2 = 1;           //Fosc/64
    //AN0 is analog other pins are digital
    ADCON1bits.PCFG3 = 1;
    ADCON1bits.PCFG2 = 1;
    ADCON1bits.PCFG1 = 1;
    ADCON1bits.PCFG0 = 0;
    ADCON0bits.ADON = 1;            //A/D module enabled
    __delay_us(20);
    ////////////////////////////////////////////////////
    /////////////////USART module///////////////////
    TRISC = 0xFF;
    SPBRG = 20;            
    TXSTAbits.BRGH = 1;     //High speed
    TXSTAbits.SYNC = 0;     //Async mode
    RCSTAbits.SPEN = 1;     //Serial port enabled
    PIE1bits.TXIE = 0;      //Tx interrupt is disabled
    TXSTAbits.TX9 = 0;      //8-bit transmission
    TXSTAbits.TXEN = 1;     //Tx enabled
    ////////////////////////////////////////////////////
    int k;
    for(k = 0; txt[k] != '\0' ; k++) {
        while(!TXSTAbits.TRMT);
        TXREG = txt[k];
    }
    
    while(1) {
        ADCON0bits.GO_nDONE = 1;                //Start A/D conversion
        while(ADCON0bits.GO_nDONE);             //Wait until the conversion is completed
        adc_data = (ADRESH << 8) + ADRESL;      //10-bit A/D result
        sprintf(txt, "Adc Value = %d\r\n", adc_data);
        for(k = 0; txt[k] != '\0' ; k++) {
            while(!TXSTAbits.TRMT);
            TXREG = txt[k];
        }
        __delay_ms(500);
    }
}
