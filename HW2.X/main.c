#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>

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

#define CS LATBbits.LATB7 




void initSPI1(){
  TRISBbits.TRISB7 = 0;
  SDI1Rbits.SDI1R= 0b0000;  //SDI for RPA1 not physical connected
  RPB5Rbits.RPB5R = 0b0011;   //(SDO for pin 14/RB5)
  SS1Rbits.SS1R= 0b0100;  // SS rpb7 pin16
  CS = 1;
  SPI1CON = 0;              // turn off the spi module and reset it
  SPI1BUF;                  // clear the rx buffer by reading from it
  SPI1BRG = 0x3;            // baud rate to 10 MHz [SPI4BRG = (80000000/(2*desired))-1]
  SPI1STATbits.SPIROV = 0;  // clear the overflow bit
  SPI1CONbits.CKE = 1;      // data changes when clock goes from hi to lo (since CKP is 0)
  SPI1CONbits.MSTEN = 1;    // master operation
  SPI1CONbits.ON = 1;       // turn on spi 1
  

}

char SPI1_IO(char write){
   SPI1BUF = write; 
   while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;}

void setVoltage(char channel, int voltage){
    unsigned short t = 0;
    
	t= channel << 15; //a is at the very end of the data transfer
	t = t | 0b0111000000000000;
    t = t | (voltage&0b0111111111111);
    
	 //rejecting excessive bits (above 10)
	
	CS = 0;
	SPI1_IO(t>>8);
    
    SPI1_IO(t&0xff);
    CS = 1;
    
    
	

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
    
    
            
    initSPI1();      
    __builtin_enable_interrupts();

    int i =0;    
    int p= 1000*10;
    int m = 3000;
    while(1) {
        _CP0_SET_COUNT(0);
    
        float a = 2048 +2000*sin(i*2*3.1415/1000*10);  //should make a 10Hz sin wave)
        float b = (4096/100)*(100-abs(i % (2*100) - 100));
        //float b = m - abs(i % (2*m) - m);
        i++;
        //if(i==1000){
        //i=0;}


        setVoltage(0,a);		//test
	//setVoltage(1,256);		//test
        setVoltage(1,b);	
        while(_CP0_GET_COUNT() < 24000) {  //check this is 1khz
        ;}
  }
    
}

