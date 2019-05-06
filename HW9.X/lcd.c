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
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations
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

void LCD_drawLetter (char letter, unsigned short x, unsigned short y ) {
    int m, n;
    char temp, currentbit;
    if ((x < 240) & (y < 320)) {
        for (m = 0; m < 5; m++){
            temp = ASCII[letter - 32][m];
            for (n = 0; n < 8; n++) { 
                currentbit = (temp >> n) & 1;
                if (currentbit == 1)
                    LCD_drawPixel(x + m, y + n, fc);
                else
                    LCD_drawPixel(x + m, y + n, bkc);
            }         
        } 
    }
}
void LCD_drawBar (unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned short color) {
    int i,j;
    for (i = x; i < x + width; i++) {
        for (j = y; j < y + height; j++) {
            LCD_drawPixel(i, j, color);
        }
    }
}

void LCD_print2 (char* m, unsigned short x, unsigned short y, unsigned short len) {
    int t = 0; 
    while (m[t]) {
        LCD_drawLetter(m[t], (x + 5 * t), y); 
        t++;
    }
    if (len > t)           //why 
        LCD_drawBar((x + 5 * t), y, 5*(len - t), 8, bkc);
}


void XPT2046_read(unsigned short *x, unsigned short *y, unsigned short *z1, unsigned short *z2){

    
   
    unsigned char hbyte, lbyte;   //high byte low byte
    CS=0;
    CS2=0;
    
    spi_io(0b10110001);             //Z1
    hbyte=spi_io(0x00);
    lbyte=spi_io(0x00);
   
    *z1 =(hbyte<<5) | (lbyte>>3);
   
    spi_io(0b11000001);             //Z2
    hbyte=spi_io(0x00);
    lbyte=spi_io(0x00);
    *z2= (hbyte<<5) | (lbyte>>3);
   

    spi_io(0b11010001);             //X
    hbyte=spi_io(0x00);
    lbyte=spi_io(0x00);
    
    *x= (hbyte<<5) | (lbyte>>3);
    
    
    spi_io(0b10010001);             //Y
    spi_io(0b11000001);             //Z2
    hbyte=spi_io(0x00);
    lbyte=spi_io(0x00);
    *z2= (hbyte<<5) | (lbyte>>3);
   

    spi_io(0b11010001);             //X
    hbyte=spi_io(0x00);
    lbyte=spi_io(0x00);
    
    *x= (hbyte<<5) | (lbyte>>3);
    
    
    spi_io(0b10010001);             //Y
    hbyte=spi_io(0x00);  //high byte
    
    lbyte=spi_io(0x00);     //low byte
       
    *y= 4096- (hbyte<<5) | (lbyte>>3);          //or ed them to git 16bits number
    
    CS=1;
    CS2=0;
    
    
    
}
 


int main() {

    
    TRISAbits.TRISA4=0;
    LATAbits.LATA4=0;
    SPI1_init() ;
    LCD_init();

    

   
    unsigned short x, y, z1, z2, x_touch, y_touch, x_touch_last, y_touch_last;
    int z, touchcount=0, z_last=0;
    
    
    
    float fps;

    char n[100];
    LCD_clearScreen(ILI9341_BLACK);
    LCD_drawBar(160,80,80,60,bc);
    LCD_drawBar(160,200,80,60,bc);
    sprintf(n, "I + + ");
    LCD_print_string(n, 180,100);
    sprintf(n, "I - - ");
    LCD_print_string(n, 180,220);
    


    while (1){
        while(_CP0_GET_COUNT()<2400000){;}
        _CP0_SET_COUNT(0);
        LATAbits.LATA4 = !LATAbits.LATA4;
        x_touch = 0;
        y_touch = 0;
        
        XPT2046_read(&x, &y, &z1, &z2);

        
        sprintf(n, "xraw: %d ",x);
        LCD_print_string(n, 10,50);
        
        sprintf(n, "yraw: %d",y);
        
        LCD_print2(n, 10,70,15);
        z=4095+z1-z2;
        sprintf(n, "z: %d",z);
        LCD_print2(n, 10, 90, 15);
        if(z>400){
            x_touch = (int)x/4095.0*240;
            y_touch = (int)y/4095.0*320;
        
        }
        
        sprintf(n,"x_touch: %d",x_touch);
        LCD_print2(n, 10, 110, 15);
        sprintf(n,"y_touch: %d",y_touch);
        LCD_print2(n, 10, 130, 15);
        
        if(z<200 && z_last>400){
        if(x_touch_last>=160 && x_touch_last<=240 && y_touch_last>=80 && y_touch_last<=140){
          touchcount+=1;
        }
        if (x_touch_last>=160 && x_touch_last<=240 && y_touch_last>=200 && y_touch_last<=260){
          touchcount-=1;
        }
      }
        z_last=z;
        x_touch_last=x_touch;
        y_touch_last=y_touch;
        sprintf(n,"I = %d",touchcount);
        LCD_print2(n, 10, 180, 15);
        
        fps=(float)24000000/_CP0_GET_COUNT();
        sprintf(n,"FPS = %5.2f",fps);
        LCD_print2(n, 10, 150, 15);

        
        
        
        


    }


}