// PROJECT DESCRIPTION
// Read from GPB0, and if the button is pushed, turn on GPA7, 
// else turn off GPA7. 
// Blink the LED that is connected directly to the PIC at some frequency as a 
// heart-beat, so that you know the PIC is running. 

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h> // lib with sprintf and sscanf
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
// function prototypes 
void delay(void);

int main(){
    __builtin_disable_interrupts(); // disable interrupts while initializing things
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; // initialize A4 as output
    LATAbits.LATA4 = 0; // A4 is initially off
        
    __builtin_enable_interrupts();
    
    i2c_master_setup(); // set up I2C1 as master, set baud rate 

    // Variables 
    unsigned char write_addr = 0b01000000;
    unsigned char read_addr = 0b01000001; // the three address pins are connected to gnd therefore low
   
    
    // REGISTER ADDRESSES (see data sheet p.12)
    unsigned char IODIRA = 0x00;
    unsigned char IODIRB = 0x01;
    unsigned char OLATA = 0x14;  // output on/off - similar to LAT
    unsigned char GPIOB = 0x13;  // will read if register is high or low
    
    // VALUES
    unsigned char output = 0x00; // make all A pins output
    unsigned char input = 0xFF;  // make all B pins input
    unsigned char High_GPA7 = 0b10000000;
    unsigned char Low_GPA7 = 0b00000000;
    //unsigned char High_GPA = 0xFF; // all A pins high
    unsigned char received_val;
    
    // INPUT/OUTPUT (for general purpose IO similar to TRIS)
    setPin(write_addr, IODIRA, output); // Make all the A pins output so IODIRA = 0x00
    setPin(write_addr, IODIRB, input); // Make all B pins input, so IODIRB = 0xFF
    


    while (1) {

        LATAbits.LATA4 = 1; 
        delay(); 
        LATAbits.LATA4 = 0; 
        delay();
        received_val = readPin(write_addr, read_addr, GPIOB);
        
        if(received_val == 0){  setPin(write_addr, OLATA, High_GPA7); }
        else{ setPin(write_addr, OLATA, Low_GPA7); }
    }
}

void delay(void){
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT() < 12000000){ 
    ; 
    }
}


