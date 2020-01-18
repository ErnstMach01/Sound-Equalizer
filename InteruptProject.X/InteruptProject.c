/*
 * File: Sound Display Interrupt Project
 * Date: December 24th, 2018
 * Name: Ernst Mach and David Mehic
 * Description: Simple project that will display the current loudness of a sound using an adc and interrupts.
 */

#include <xc.h>
#define _XTAL_FREQ 4000000
// PIC16F690 Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = INTRCIO   // (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

int pattern;                   //Value to store what lights should be turned on

//Interrupt routine
void interrupt isr ()
{
    int range = 132;            //Set the lowest range value since sound waves will produce a specific voltage range when converted
    if (ADRESH >= 0 && ADRESH < (range-2)) {            //If the value produced is in the lowest range
        pattern = 0b00000000;                           //Turn off all the lights
    }
    
    if(ADRESH >= (range-2) && ADRESH < range){          //If a voltage is 2 points greater than the previous range 
        pattern = 0b00000001;                           //Turn on 1 light
    } 
    
    if(ADRESH >= range && ADRESH < (range+2)){          //If a voltage is 2 points greater than the previous range 
        pattern = 0b00000011;                           //Turn on 2 lights
    }
    
    if(ADRESH >= (range+2) && ADRESH < (range+4)){      //If a voltage is 2 points greater than the previous range 
        pattern = 0b00000111;                           //Turn on 3 lights
    }
    
    if(ADRESH >= (range+4) && ADRESH < (range+6)){      //If a voltage is 2 points greater than the previous range 
      pattern = 0b00001111;                             //Turn on 4 lights
    }

    if(ADRESH >= (range+6) && ADRESH < (range+8)){      //If a voltage is 2 points greater than the previous range 
        pattern = 0b00011111;                           //Turn on 5 lights
    }

    if(ADRESH >= (range+8) && ADRESH < (range+10)){     //If a voltage is 2 points greater than the previous range 
        pattern = 0b00111111;                           //Turn on 6 lights
    }

    if(ADRESH >= (range+10)  && ADRESH < (range+12)){   //If a voltage is 2 points greater than the previous range 
        pattern = 0b01111111;                           //Turn on 7 lights
    }

    if(ADRESH > (range+12)){                            //If a voltage is anything greater than the previous range
        pattern = 0b1111111;                            //Turn on all the lights
    }    
    
    PIR1bits.ADIF = 0;                                  //Conversion completed
    return;
}

void main(void) {
    TRISA= 0xFF;                //Sets PORT A to inputs
    TRISB= 0xFF;                //Sets PORT B to inputs
    TRISC= 0x00;                //Set C to outputs
    PORTC= 0;                   //Turn off all the lights
    
    ANSEL= 0x00;                //Turn off all the analog ports  
    ANSELbits.ANS2=1;           //Turn on the specific analog port A2
    
    ADCON0bits.ADFM=0;          //Left justified in 2 bits
    ADCON0bits.VCFG=0;          //5 volt reference
    ADCON1bits.ADCS= 0b001;     //Set the speed
    
    ADCON0bits.CHS=2;           //Select the chanel
    ADCON0bits.ADON=1;          //Turn the AtoD converter on
    
    INTCONbits.RABIE = 1;       //Enable the portal interrupt
    INTCONbits.RABIF = 0;       //Reset the interrupt flag
    INTCONbits.GIE = 1;         //Set the Global Interrupt Enable
    INTCONbits.PEIE = 1;        //Turns on the peripheral interrupts
    PIE1bits.ADIE = 1;          //Enables the A/D Converter interrupt
    
    while(1) {
        __delay_ms(100);        //Wait 100 ms
        ADCON0bits.GO = 1;      //Start the adc conversion
        PORTC = pattern;        //Display the pattern
    }
    return;
}