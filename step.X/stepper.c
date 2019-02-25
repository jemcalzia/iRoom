
#include "stepper.h"
#include <stdint.h>
#include <xc.h>
#define _XTAL_FREQ 8000000


void 
full_step(uint8_t speed, uint16_t steps)
    {
    static char i; 
    for(i = 0; i<=steps;i++)
    {
        if ((i % 4) = 1){
            PORTE = 3; 
            __delay_ms(speed);
        }
        if ((i % 4) = 2){
            PORTE = 6; 
            __delay_ms(speed);
        }
        if ((i % 4) = 3){
            PORTE = 12; 
            __delay_ms(speed);
        }
        if ((i % 4) = 0){
            PORTE = 9; 
            __delay_ms(speed);
        }
    }
    
    }
void 
wave_step(uint8_t speed, uint16_t steps)
    {
    static char i; 
    for(i = 0; i<=steps;i++)
    {
        if ((i % 4) = 1){
            PORTE = 1; 
            __delay_ms(speed);
        }
        if ((i % 4) = 2){
            PORTE = 2; 
            __delay_ms(speed);
        }
        if ((i % 4) = 3){
            PORTE = 4; 
            __delay_ms(speed);
        }
        if ((i % 4) = 0){
            PORTE = 8; 
            __delay_ms(speed);
        }
    }
    }