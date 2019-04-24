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



void LCD_print_letter(char letter, int x, int y){
    int i;
    int j;
    for (i =0; i<5; i++){
        char col = ASCII[letter-0x20][i];
        for (j =0; j<8; j++){
            char lsbit=(col>>j)&1;
            if (lsbit ==1){
                LCD_drawPixel(x+i, y+j, fc);
            }
            else{
                LCD_drawPixel(x+i, y+j, bkc);
            }
        }
    }
}

void LCD_print_string(char *m, int x, int y){
    int i=0;
    while (m[i]){
    LCD_print_letter(m[i], x+(i*5), y);
    i++;
    }
}

void LCD_print_bar(int x, int y){
    int i;
    for (i=0; i<100; i++){
        LCD_drawPixel(x+i,y,fc);
        LCD_drawPixel(x+i,y+10,fc);
    }
    for (i=0; i<10; i++){
        LCD_drawPixel(x,y+i,fc);
        LCD_drawPixel(x+100,y+i,fc);
    }

}

void LCD_bar_progress(int x, int y, int i){
    int j;
    int z;
    int s;
    for(j=1; j<i;j++){
        LCD_drawPixel(x+j,y+2,bc); //horizontal

        for (z=1; z<10;z++){
            LCD_drawPixel(x+j,y+z,bc);  //vertical

        }
    }
    for(s=i+1;s<100; s++ ){
        LCD_drawPixel(x+s,y+2,bkc);
        for (z=1; z<10;z++){
            LCD_drawPixel(x+s,y+z,bkc);  //vertical
        }
    }
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
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;
    SPI1_init() ;
    LCD_init();

    LCD_clearScreen(ILI9341_BLACK);

    __builtin_enable_interrupts();
    int d=0;
    char m[100];
    float fps;

    char n[20];
    LCD_print_bar(30, 50);


    while (1){
        while(_CP0_GET_COUNT()<2400000){;}
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = !LATAbits.LATA4;
        sprintf(m, "Hello World! %d",d);
        d++;
        LCD_print_string(m, 28, 32);
        LCD_bar_progress(30,50,d);

        fps=1/(_CP0_GET_COUNT()*42/(1000000000.0));
        sprintf(n, "FPS: %f",fps);
        LCD_print_string(n, 30,70 );
        if(d>=100){
        d=0;
        }


    }
}