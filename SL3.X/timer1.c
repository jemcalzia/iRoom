

#include "timer1.h"
#include <stdint.h>
#include <xc.h>
uint8_t t1_count; 
void
timer1_begin(uint8_t offset, uint8_t prescaler)
    {
    T1CONbits.T1GINV = 0;
    T1CONbits.TMR1GE = 0;
    T1CONbits.T1CKPS = prescaler; 
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
    
    INTCONbits.PEIE = 1; 
    PIR1bits.TMR1IF = 0; 
    PIE1bits.TMR1IE = 1; 
    INTCONbits.GIE = 1;
    TMR1 = offset;            
    }

