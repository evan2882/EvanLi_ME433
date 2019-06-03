#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>
#include "ili9341.h"
#include "stdio.h"


// DEVCFG0
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

#define fc ILI9341_WHITE
#define bkc ILI9341_BLACK
#define bc ILI9341_RED


int count_flag=0;





void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) {

IFS0bits.T3IF = 0;
LATBbits.LATB9 = !LATBbits.LATB9;

if (count_flag>99){
    count_flag=0;
}
OC4RS =24*count_flag; 
        //24*count_flag;
count_flag++;

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
    RPA4Rbits.RPA4R=0b0101;
   
    DDPCONbits.JTAGEN = 0;
    TRISBbits.TRISB9=0;
    TRISAbits.TRISA4=0;
    
    
    
    SPI1_init() ;
    LCD_init();

    LCD_clearScreen(ILI9341_BLACK);
    T2CONbits.TCKPS = 0; // Timer2 prescaler N=1 (1:1)

    PR2 = 2399; // PR = PBCLK / N / desiredF - 1

    TMR2 = 0; // initial TMR2 count is 0

    OC4CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults

    OC4RS = 0; // duty cycle

    OC4R = 0; // initialize before turning OC1 on; afterward it is read-only

    T2CONbits.ON = 1; // turn on Timer2

    OC4CONbits.ON = 1; // turn on OC1
    PR3 = 59999;
    TMR3=0;
    T3CONbits.TCKPS = 0b01;
    T3CONbits.ON = 1;
    IPC3bits.T3IP=5;
    IPC3bits.T3IS=0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE=1;
    
            
    __builtin_enable_interrupts();
    int d=0;
    int i=0;
    char m[100];
    float fps;

    char n[20];
   


    while (1){
        while(_CP0_GET_COUNT()<2400000){;}
        _CP0_SET_COUNT(0);
        
        for (i=0; i<100; i++){
       //LATAbits.LATA4=0;
        
        LCD_drawPixel(20+i,30+5*sin(i),fc);
        LCD_drawPixel(20+i,31+5*sin(i),fc);
        
        }   
        
        
        


    }
}