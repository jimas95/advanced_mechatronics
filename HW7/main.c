#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include <math.h>       // for sin wave
#include "spi.h"

// DEVCFG0
// file:///Applications/microchip/xc32/v2.50/docs/config_docs/32mx170f256b.html
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect
// DEVCFG1
// use internal resonator instead of primary one (HW4)
#pragma config FNOSC = FRCPLL // use INTERNAL oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // INTERNAL RC
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%
// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz
// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

// CONSTANTS
#define TABLE_SIZE 100 
#define TWO_PI (3.14159 * 2)

//void initSPI(); 
unsigned char spi_io(unsigned char o);  // read and write happen simotaneously for spi


int main() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    // INITIALIZE PINS
    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; // initialize A4 as output
    LATAbits.LATA4 = 0; // A4 is initially off
    TRISBbits.TRISB4 = 1; // initialize B4 as input
    
    initSPI(); // call function 
    __builtin_enable_interrupts();
      
    // COUNTER IN INF WHILE LOOP
    unsigned char i = 0;
    
    // WAVE AMPLITUDES
    unsigned short A1 = 1024/2;
    
    // CHANNEL
    unsigned char c_A = 0;      // channel 0 = A or 1 = B
    unsigned char c_B = 1;
    
    // PRINTED VALUE
    unsigned short S;           // sine 
    unsigned short T;           // triangle
             
    
    // triangle wave
    unsigned short triArray[TABLE_SIZE];
    
    // sine wave
    unsigned short sinArray[TABLE_SIZE]; 
    float phaseIncrement = 2*TWO_PI/TABLE_SIZE;
    float currentPhase = 0.0;
    int j;
    for(j = 0; j < TABLE_SIZE; j ++)
    {
        sinArray[j] = (unsigned short)A1*sin(currentPhase)+A1;
        currentPhase += phaseIncrement; // increments to 2pi --> one period
        
        if(j<50){
            triArray[j] = 2*A1*j/50.0;
        }
        else{
            triArray[j] = 2*A1 - 2*A1*(j-50)/50.0;
        }
        
    }
    
    
    while(1) {
        // write one byte over SPI1
        S = voltage(sinArray[i],c_A);      // voltage, channel (0 = A or 1 = B) 
        T = voltage(triArray[i],c_B);
                
        LATAbits.LATA0 = 0;             // BRING CS low
        // if wanted to read something --> var = spi_io(i)
        spi_io(S>>8);                   // first 8 bits of P (leftmost)
        spi_io(S);                      // last 8 bits of P
        LATAbits.LATA0 = 1;             // bring CS high
        
        LATAbits.LATA0 = 0;             // BRING CS low
        spi_io(T>>8);                   // first 8 bits of P (leftmost)
        spi_io(T);                      // last 8 bits of P
        LATAbits.LATA0 = 1;             // bring CS high
        
        // reset index for samples/waveform list
        i++;
        if (i == TABLE_SIZE){
            i = 0;
        }
        
        //delay
        _CP0_SET_COUNT(0);              // sets the value of the core timer
        while (_CP0_GET_COUNT() < 240000) {
            ;// 1/100 second delay
        }
        
    }

}




	