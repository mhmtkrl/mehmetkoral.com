/*
 * File:   i2c.c
 * Author: koral
 *
 * Created on 31 January 2021, 20:38
 */


#include <xc.h>
#include "i2c.h"

void i2c_init(void) {
    TRISCbits.TRISC3 = 1;      
    TRISCbits.TRISC4 = 1;   
    SSPCONbits.SSPEN = 1;                   //SSP is enabled
    //I2C Master Mode -> clock = Fosc/(4*(SSPADD+1))
    SSPCONbits.SSPM3 = 1;
    SSPCONbits.SSPM2 = 0;   
    SSPCONbits.SSPM1 = 0;
    SSPCONbits.SSPM0 = 0;
    //Fosc = 20 MHz, clock = 1kHz -> SSPADD = 49
    SSPADD = 49;   
}
//Start Condition
void i2c_start(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       //MSSP is in IDLE mode
    SSPCON2bits.SEN = 1;
    while(SSPCON2bits.SEN);
}
//Stop Condition
void i2c_stop(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       //MSSP is in IDLE mode
    SSPCON2bits.PEN = 1;
    while(SSPCON2bits.PEN);
}

void i2c_write(int data) {
    SSPBUF = data;
    while(SSPSTATbits.BF);
    while (SSPSTATbits.R_nW);
}

int i2c_read(void) {
    int data;
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       //MSSP is in IDLE mode
    SSPCON2bits.RCEN = 1;
    while(!SSPCON2bits.RCEN);
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       //MSSP is in IDLE mode
    data = SSPBUF;
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));       //MSSP is in IDLE mode
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    return data;
}

/*
 * 21462D.pdf, section 3.1.2 I2C Slave Address, page 6
 * TC74 device address is 0b1001 101x
 * To write purpose Read bit is clear so address become 0b1001 1010 = 0x9A
 * To read purpose Read bit is set so address become 0b1001 1011 = 0x9B
 * 
 * Figure 3.1 shows the writing and reading sequences in page 7
 * To Write
 *  Start
 *  Slave Addr + Write bit = 0x9A
 *  Wait for ACK from slave device
 *  Select which register we want to access
 *  Wait for ACK from slave device
 *  Write data to the register we accessed
 *  Wait for ACK from slave device
 *  Stop
 * 
 * To Read
 *  Start
 *  Slave Addr + Write bit = 0x9A
 *  Wait for ACK from slave device
 *  Select which register we want to access
 *  Wait for ACK from slave device
 *  Stop
 *  Start
 *  Slave Addr + Read bit = 0x9B
 *  Wait for ACK from slave device
 *  Wait for Data from slave device
 *  NACK (SSPCON2bits.ACKDT = 1 represent this situation)
 *  Stop
 * 
 */

/*
 * Put device the into a low power mode
 * Standby mode is enabled by setting
   the SHDN bit in the CONFIG register.
 */
void tc74_idle_mode(int state) {
    i2c_start(); 
    i2c_write(0x9A);
    i2c_write(0x01);
    if(state) i2c_write(0x80);
    else      i2c_write(0x00);
    i2c_stop();
}

int8_t tc74_read_temp(void) {
    int8_t temp_value;
    i2c_start(); 
    i2c_write(0x9A);
    i2c_write(0x00);
    i2c_stop();
    i2c_start();
    i2c_write(0x9B);
    temp_value = i2c_read();
    i2c_stop();
    return temp_value;
}

uint8_t tc74_temp_ready(void) {
    uint8_t tc74_ready;
    i2c_start(); 
    i2c_write(0x9A);
    i2c_write(0x01);
    i2c_stop();
    i2c_start();
    i2c_write(0x9B);
    tc74_ready = i2c_read();
    i2c_stop(); 
    return tc74_ready;
}
