/*
 * File:   main.c
 * Author: koral
 *
 * Created on 30 January 2021, 22:55
 */


#include <xc.h>
#include "fuses.h"
#include <math.h>

float i = 0;
int sinValue = 0;

void main(void) {
    TRISD = 0x00;
    PORTD = 0x00;
    /////////////////SPI module///////////////////
    /*
     * RC3 -> SCK
     * RC5 -> SDO
     */
    TRISC = 0xFF;    
    TRISC &= ~(1ul << 3);              //RC3 is output
    TRISC &= ~(1ul << 5);              //RC5 is output
    SSPSTATbits.SMP = 0;               //Input data sampled at end of data output time
    SSPSTATbits.CKE = 0;               //Transmit occurs on transtion from idle to active clock state
    //The rest of SSPSTAT's bits are only readlable
    SSPCONbits.CKP = 0;                //Clock Polarity is idle state for clock is a low level
    SSPCONbits.SSPM3 = 0;              //Clock is Fosc/4
    SSPCONbits.SSPM2 = 0;
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM0 = 0;
    SSPCONbits.CKP = 1;
    SSPCONbits.SSPEN = 1;  
    ////////////////////////////////////////////////////

    while(1) {   
        sinValue = (int)((sin(0.63*i)+1)*2048);
        PORTDbits.RD0 = 0;
        //0x0011000 -> Gain = 1
        //Output power is enabled
        //12-bit D/A value
        SSPBUF = 0x30 | (sinValue >> 8);
        while(SSPSTATbits.BF == 0);
        SSPBUF = sinValue & 0xFF;
        while(SSPSTATbits.BF == 0);
        PORTDbits.RD0 = 1;
         i+=0.1;
         if(i > 4096.0) i = 0;
    }
}