/*
 * File:   main.c
 * Author: koral
 *
 * Created on 28 January 2021, 21:11
 */


#include <xc.h>
#include "fuses.h"

#define rx_isr PIR1bits.RCIF

unsigned char txt[] = "\r\nEmbedded Systems!";
unsigned char data;

void __interrupt() my_isr() {
    //Basic ECHO service
    //Send back what you receive
    if(rx_isr) {
        data = RCREG;
        TXREG = data;
    }
}

void main(void) {
    //RC6->TX & RC7->RX
    TRISC = 0xFF;
    ////////////Baudrate = 57600//////////////
    SPBRG = 20;            
    ///////////////////////////////////////////
    ////////////TX Process////////////////////
    TXSTAbits.BRGH = 1;     //High speed
    TXSTAbits.SYNC = 0;     //Async mode
    RCSTAbits.SPEN = 1;     //Serial port enabled
    PIE1bits.TXIE = 0;      //Tx interrupt is disabled
    TXSTAbits.TX9 = 0;      //8-bit transmission
    TXSTAbits.TXEN = 1;     //Tx enabled
    ///////////////////////////////////////////
    ////////////Rx Process////////////////////
    RCSTAbits.RX9 = 0;      //8-bit reception
    RCSTAbits.CREN = 1;     //Enable continuous receive
    PIE1bits.RCIE = 1;      //Rx interrupt is enabled
    ///////////////////////////////////////////
    ////////////ISR////////////////////
    INTCONbits.PEIE = 1;    //Enables periphereal interrupts
    INTCONbits.GIE = 1;     //Activates all enabled interrupt sources
    ///////////////////////////////////////////
    TRISD = 0xFC;
    PORTD = 0x00;
    
    int k;
    for(k = 0; txt[k] != '\0' ; k++) {
        //Transmit shift register is empty?
        while(!TXSTAbits.TRMT);
        TXREG = txt[k];
    }
    
    while(1) {
        if(data == 'r') PORTD = 0x01;
        if(data == 'y') PORTD = 0x02;
    }
}
