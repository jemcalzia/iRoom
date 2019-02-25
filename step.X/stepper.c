#define _XTAL_FREQ 8000000
#include "stepper.h"
#include <stdint.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

void 
full_step(uint16_t steps)
    {
    static uint16_t i; 
    while(i<steps)
    {
        if ((i % 4) == 1){
            PORTD = 3; 
            __delay_ms(5);
        }
        if ((i % 4) == 2){
            PORTD = 6; 
            __delay_ms(5);
        }
        if ((i % 4) == 3){
            PORTD = 12; 
            __delay_ms(5);
        }
        if ((i % 4) == 0){
            PORTD = 9; 
            __delay_ms(5);
        }
        i++;
    }
    }

void 
wave_step(uint16_t steps)
    {
    static uint16_t i, a;
    i = 0; 
    while(i<steps)
    {
        a = i%4;
        if (a == 1){
            PORTD = 1; 
            __delay_ms(5);
        }
        if (a == 2){
            PORTD = 2; 
            __delay_ms(5);
        }
        if (a == 3){
            PORTD = 4; 
            __delay_ms(5);
        }
        if (a == 0){
            PORTD = 8; 
            __delay_ms(5);
        }
        i++;
        }
    }

void 
full_rev(uint16_t steps)
    {
    static char i; 
    while(i<steps)
    {
        if ((i % 4) == 1){
            PORTD = 9; 
            __delay_ms(5);
        }
        if ((i % 4) == 2){
            PORTD = 12; 
            __delay_ms(5);
        }
        if ((i % 4) == 3){
            PORTD = 6; 
            __delay_ms(5);
        }
        if ((i % 4) == 0){
            PORTD = 3; 
            __delay_ms(5);
        }
        i++;
    }
    }
   

void 
wave_rev(uint16_t steps)
    {
    static uint16_t i, a;
    i = 0; 
    while(i<steps)
    {
        a = i%4;
        if (a == 1){
            PORTD = 8; 
            __delay_ms(5);
        }
        if (a == 2){
            PORTD = 4; 
            __delay_ms(5);
        }
        if (a == 3){
            PORTD = 2; 
            __delay_ms(5);
        }
        if (a == 0){
            PORTD = 1; 
            __delay_ms(5);
        }
        i++;
        }
    }