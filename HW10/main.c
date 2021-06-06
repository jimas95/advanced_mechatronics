#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro
#include<stdio.h>
#include "ws2812b.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use internal pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // RC mode
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


int main() {
    
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    
    // do your TRIS and LAT commands here
    TRISBbits.TRISB4 = 1; //B4 is input
    TRISAbits.TRISA4 = 0; //A4 is output
    LATAbits.LATA4 = 0; //A4 is low
    
    
    ws2812b_setup();
    __builtin_enable_interrupts();
    
    //initialize the hues here
    float hue_1 = 0;
    float hue_2 = 90;
    float hue_3 = 180;
    float hue_4 = 270;

    //reset the lights before use
    LATBbits.LATB6 = 0; 
    
    // set timer initial to 0
    TMR2 = 0;
    
    int LED_num = 4; // total number of LEDs
    float incr = 0.5;
    int counter = 2;
    
    while (1) {
        wsColor c1 = HSBtoRGB(hue_1,1,0.1);
        wsColor c2 = HSBtoRGB(hue_2,1,0.1);
        wsColor c3 = HSBtoRGB(hue_3,1,0.1);
        wsColor c4 = HSBtoRGB(hue_4,1,0.1);
        
       if (counter%2 == 0){
           c1 = c4;
           c2 = c3;
           c3 = c2;
           c4 = c1;
       } 

        
        wsColor color[4] = {c4, c3, c2, c1}; 
        
        hue_1 = hue_1 + incr;
        hue_2 = hue_2 + incr;
        hue_3 = hue_3 + incr;
        hue_4 = hue_4 + incr;
      
 
        if (hue_1 > 360){
            hue_1 = 0;
        }
        
        if (hue_2 > 360){
            hue_2 = 0;
        }
        
        if (hue_3 > 360){
            hue_3 = 0;
        }
        
        if (hue_4 > 360){
            hue_4 = 0;
        }
        
        ws2812b_setColor(color, LED_num);
        
        TMR2 = 0;
        counter = counter + 1;
        
        while(TMR2 < 50000){
        }  // delay
    }
    
    
}