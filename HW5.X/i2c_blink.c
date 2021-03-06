#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include "i2c_master_noint.h"
#include "stdio.h"
// Demonstrate I2C by having the I2C1 talk to I2C5 on the same PIC32 (PIC32MX795F512H)
// Master will use SDA1 (D9) and SCL1 (D10).  Connect these through resistors to
// Vcc (3.3 V) (2.4k resistors recommended, but around that should be good enough)
// Slave will use SDA5 (F4) and SCL5 (F5)
// SDA5 -> SDA1
// SCL5 -> SCL1
// Two bytes will be written to the slave and then read back to the slave.
#define SLAVE_ADDR 0x32
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // no boot write protect
#pragma config CP = OFF // no code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable secondary osc
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1 // use slowest wdt
#pragma config WINDIS = OFF // wdt no window mode
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
#pragma config UPLLIDIV = DIV_2 // divider for the 8MHz input clock, then multiplied by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = ON // allow multiple reconfigurations
#pragma config IOL1WAY = ON // allow multiple reconfigurations
#pragma config FUSBIDIO = ON // USB pins controlled by USB module
#pragma config FVBUSONIO = ON // USB BUSON controlled by USB module
void initI2C2(){
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;
    i2c_master_setup();
    
}

void initExpander(){
    i2c_master_start();
    i2c_master_send(0x40);  //the address 0100A2A1A0W/R
    i2c_master_send(0x00);  //IODIR address is 0.
    i2c_master_send(0xF0);  // Input/Output  1 Input O Output
    i2c_master_stop();
}

char getExpander(){
    i2c_master_start(); // make the start bit

    i2c_master_send(0x40); // the address 0100A2A1A0W/R

    i2c_master_send(0x09); // the register to read from

    i2c_master_restart(); // make the restart bit

    i2c_master_send(0x41); // write the address, shifted left by 1, or'ed with a 1 to indicate reading

    char r = i2c_master_recv()>>7; // save the value returned

    i2c_master_ack(1); // make the ack so the slave knows we got it

    i2c_master_stop(); // make the stop bit
    return r;
}

void setExpander(char level){
    i2c_master_start(); // make the start bit

    i2c_master_send(0x40); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

    i2c_master_send(0x0A); 
    if (level ==1){
        i2c_master_send(0b1111);}
    if (level ==0){
        i2c_master_send(0b00);}
    i2c_master_stop();
        
}

int main() {

    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;
    initI2C2();
    initExpander();
    

    while(1) {
        _CP0_SET_COUNT(0);
        
        while (_CP0_GET_COUNT()<6000000){
            ;}       //delay for 0.25s
        LATAbits.LATA4=!LATAbits.LATA4;
        //setExpander(1);
        if(getExpander()<1){
            setExpander(1);
        }
        else {
            setExpander(0);
        }
    }
   
}



 