#ifndef ACC_H__
#define ACC_H__

void initI2C2();
char get_who_am_i();
void acc_init();
void I2C_read_multiple(unsigned char register1, unsigned char * data, int length);

#endif
