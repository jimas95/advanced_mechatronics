#include<xc.h>           // processor SFR definitions
// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn off analog pins; on A port
    ANSELA = 0;  // make the pins digital - if we didnt analog input pin by default (1 for analog)
    
    // Make an output pin for CS
    TRISAbits.TRISA0 = 0; 
    LATAbits.LATA0 = 1; 
    // Set SDO1
    RPA1Rbits.RPA1R = 0b0011; // pin A1 SDO1 
    // Set SDI1
    SDI1Rbits.SDI1R = 0b0001; // B5

    // setup SPI1****
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1000;//12000;//1000; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation (PIC is master...controls the clock)
    SPI1CONbits.ON = 1; // turn on spi 
}


// send a byte via spi and return the response
unsigned char spi_io(unsigned char o) {
  // send and receive buffer
  SPI1BUF = o; // put data here we want to send AND put data that gets returned here
  //PI1BUF = 0b;
  while(!SPI1STATbits.SPIRBF) { // wait to receive the byte
    ;
  }
  return SPI1BUF;
}

// set 10 bit voltage value
unsigned short voltage(unsigned short v, unsigned char c){
    unsigned short p;
    p = c <<15;             // channel (15 zeros behind it)
    p = p|(0b111<<12);      // 12 zeros behind bit 14,13,12 (BIT 14-12 SEE HW7 NOTES or data sheet)
    p = p|(v<<2);           // 10 bit voltage
}
