/*
 Autor: José Molina 
 Date: 21/02/2019
 
 */

#include "dht11_h.h"
#include <stdint.h>
#include <xc.h>
#define _XTAL_FREQ 8000000

void
dht11_begin(void)
    {
    DIR = 0; // Data pin as output
    DATA = 0; 
    __delay_ms(18);
    DATA = 1;
    __delay_us(20);
    DIR = 1; //Data pin as input
    }

void
dht11_check(void)
    {
    while(DATA); 
    while(!DATA);
    while(DATA);
    }
uint8_t 
dht11_read(void)
    {
    static uint8_t i = 0, temp; 
    for(i = 0; i < 8 ; i++)
    {
        while(!DATA);
        __delay_us(30);
        if (DATA)
        {
            temp = ((temp<<1) | 1);    
        }
        else
          temp = (temp<<1);
        while(DATA);  
    }
    return temp; 
    }
    