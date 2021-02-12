/*
 * File:   uart.c
 * Author: koral
 *
 * Created on 31 January 2021, 20:24
 */


#include <xc.h>
#include "uart.h"

void UART_Init(void) {
    /////////////////USART module///////////////////
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC7 = 1;
    SPBRG = 20;            
    TXSTAbits.BRGH = 1;     //High speed
    TXSTAbits.SYNC = 0;     //Async mode
    RCSTAbits.SPEN = 1;     //Serial port enabled
    PIE1bits.TXIE = 0;      //Tx interrupt is disabled
    TXSTAbits.TX9 = 0;      //8-bit transmission
    TXSTAbits.TXEN = 1;     //Tx enabled
    ////////////////////////////////////////////////////
}

void UART_Transmit_String(char *txt) {
    int k;
    for(k = 0; txt[k] != '\0' ; k++) {
        while(!TXSTAbits.TRMT);
        TXREG = txt[k];
    }
}