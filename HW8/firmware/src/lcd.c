#include<xc.h>           // processor SFR definitions
#include <math.h>
#include "ili9341.h"
#include "lcd.h"




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

void LCD_xbar_progress(short k){
    int j;
    int z;
    int s;
    int i;
    for (i=0; i<240; i++){              //draw bar shape
        LCD_drawPixel(i,155,fc);
        LCD_drawPixel(i,165,fc);
    }
    for (i=0; i<10; i++){
        LCD_drawPixel(0,155+i,fc);
        LCD_drawPixel(240,155+i,fc);
    }
    if (k>=0){
        for(j=1; j<k;j++){
        LCD_drawPixel(120+j,156,bc); //horizontal

        for (z=1; z<9;z++){
            LCD_drawPixel(120+j,156+z,bc);  //vertical

        }
    }
        for(s=k+1;s<119; s++ ){
        LCD_drawPixel(120+s,156,bkc);
        for (z=1; z<9;z++){
            LCD_drawPixel(120+s,156+z,bkc);  //vertical
        }
    }
    }
        
    if (k<0){
        k=-k;
        for(j=1; j<k;j++){
        LCD_drawPixel(120-j,156,bc); //horizontal

        for (z=1; z<9;z++){
            LCD_drawPixel(120-j,156+z,bc);  //vertical

        }
    }
        for(s=2;s<120-k; s++ ){
        LCD_drawPixel(s,156,bkc);
        for (z=1; z<9;z++){
            LCD_drawPixel(s,156+z,bkc);  //vertical
        }
        }
        
    }    
    

}

void LCD_ybar_progress(short k){
    int j;
    int z;
    int s;
    int i;
    
    for (i=0; i<360; i++){              //draw bar shape
        LCD_drawPixel(115,i,fc);
        LCD_drawPixel(125,i,fc);
    }
    for (i=0; i<10; i++){
        LCD_drawPixel(115+i,0,fc);
        LCD_drawPixel(115+i,320,fc);
    }
    if (k>=0){
        for(j=1; j<k;j++){
        LCD_drawPixel(116,155+j,bc); //horizontal

        for (z=1; z<9;z++){
            LCD_drawPixel(116+z,155+j,bc);  //vertical

        }
    }
        for(s=k+1;s<158; s++ ){
        LCD_drawPixel(116,160+s,bkc);
        for (z=1; z<9;z++){
            LCD_drawPixel(116+z,160+s,bkc);  //vertical
        }
    }
    }
      
    if (k<0){
        k=-k;
        for(j=1; j<k;j++){
        LCD_drawPixel(116,160-j,bc); 

        for (z=1; z<9;z++){
            LCD_drawPixel(116+z,160-j,bc);  

        }
    }
        for(s=2;s<160-k; s++ ){
        LCD_drawPixel(116,s,bkc);
        for (z=1; z<9;z++){
            LCD_drawPixel(116+z,s,bkc);  //vertical
        }
        }
        
    }    
   
}


