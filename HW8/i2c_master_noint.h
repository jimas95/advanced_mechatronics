#ifndef I2C_MASTER_NOINT_H__
#define I2C_MASTER_NOINT_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts

#include <xc.h>

void i2c_master_setup(void); // set up I2C1 as master, set baud rate 
void i2c_master_start(void); // send a START signal
void i2c_master_restart(void); // send a RESTART signal
void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char i2c_master_recv(void); // receive a byte of data
void i2c_master_ack(int val); // send an ACK (0) or NACK (1), when RECIEVING you need to acknowledge that you got what youre reading
void i2c_master_stop(void); // send a stop

// general puurpose write and read functions
void setPin(unsigned char address, unsigned char reg, unsigned char value);
unsigned char readPin(unsigned char write_addr, unsigned char read_addr, unsigned char reg);
#endif

// Write
// 1. start 
// 2. address: 0b01000000
// 3. data: reg
// 4. data: value
// 5. stop 

// Read 
// 1. start
// 2. address: 0b01000000
// 3. reg to read from
// 4. restart
// 5. address: 0b01000001
// 6. receive value
// 7. Ack
// 8. stop

