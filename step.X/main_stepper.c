
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 8000000
#define EN RA1
#define RS RA2
#include <stdint.h>
#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include "ADC.h"
#include "LCD8bit.h"
#include "Oscilator.h"
#include "stepper.h"
uint8_t adc_read, i;
uint16_t step, theta, step_last, delta;
char show[15], test[20];

void main(void)
    {
    oscilator_begin(7);
    ADC_begin(0, 2);
    TRISB = 0;
    TRISE = 1; 
    ANSELbits.ANS1 = 0;
    ANSELbits.ANS2 = 0;
    TRISA1 = 0;
    TRISA2 = 0;
    PORTA = 0;
    PORTB = 0;
    TRISD = 0;
    PORTD = 0;
    LCD8_begin();
    LCD8_clear();
    LCD8_set_cursor(1, 1);
    LCD8_strWrite("holaaa");
    __delay_ms(5000);
    while (1) {
        LCD8_clear();
        adc_read = ADC_conversion();
        step = adc_read * 8U;
        theta = step / 6;
        sprintf(show, "Theta = %d", theta);
        sprintf(test, "Moviendo: %d", step);
        LCD8_set_cursor(1, 1);
        LCD8_strWrite(show);

        LCD8_set_cursor(2, 1);
        LCD8_strWrite(test);
        
        /*if(RE3){
            while(RE3);
            wave_rev(step);//2048
            PORTD = 0;
        }*/
        if(RE3){
            while(RE3);
            wave_step(step);
            PORTD = 0; 
        }


    }
    }