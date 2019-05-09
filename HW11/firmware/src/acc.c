

#include "acc.h"
#include "i2c_master_noint.h" 
#include <xc.h>

void initI2C2(){
    ANSELBbits.ANSB2=0;
    ANSELBbits.ANSB3=0;
    i2c_master_setup();

}



char get_who_am_i(){
    i2c_master_start(); // make the start bit

    i2c_master_send(0b11010110); // the address 0b1101011 + 0 for write 1 for read

    i2c_master_send(0b1111); // the register to read from

    i2c_master_restart(); // make the restart bit

    i2c_master_send(0b11010111); // write the address, shifted left by 1, or'ed with a 1 to indicate reading

    char r = i2c_master_recv(); // save the value returned

    i2c_master_ack(1); // make the ack so the slave knows we got it

    i2c_master_stop(); // make the stop bit
    return r;
}

void acc_init(){
    //turn on CTRL1_XL register
    i2c_master_start(); // make the start bit

    i2c_master_send(0b11010110); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

    i2c_master_send(0x10); // send CTRL1_XL register

    i2c_master_send(0b10000010); // set the value of the register

    i2c_master_stop(); // make the stop bit

    i2c_master_start(); // make the start bit

    i2c_master_send(0b11010110); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

    i2c_master_send(0x11); // send CTRL2_G register

    i2c_master_send(0b10001000); // set the value of the register

    i2c_master_stop(); // make the stop bit

    i2c_master_start(); // make the start bit

    i2c_master_send(0b11010110); // write the address, shifted left by 1, or'ed with a 0 to indicate writing

    i2c_master_send(0x12); // send CTRL3_CC register

    i2c_master_send(0b100); // set the value of the register

    i2c_master_stop(); // make the stop bit
}

void I2C_read_multiple(unsigned char register1, unsigned char * data, int length){
    int i;
    i2c_master_start(); // make the start bit

    i2c_master_send(0b11010110); // the address 0 for write 1 for read

    i2c_master_send(register1); // the register to read from

    i2c_master_restart(); // make the restart bit

    i2c_master_send(0b11010111); // write the address, shifted left by 1, or'ed with a 1 to indicate reading

    for (i=0; i<length-1; i++){
        data[i] = i2c_master_recv(); // save the value returned
        i2c_master_ack(0);
    }


    data[i] = i2c_master_recv();
    i2c_master_ack(1); // make the ack so the slave knows we got it

    i2c_master_stop(); // make the stop bit

}


