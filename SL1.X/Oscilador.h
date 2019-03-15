//******************************************************************************
/* 
 * File:   Oscilador.h
 * Author: Pablo Mazariegos
 * Librería para configurar el oscilador interno
 * PIC16F887
 * Created on 15 de enero de 2019, 11:06 AM
 */
//******************************************************************************
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)

//******************************************************************************
// Función para inicializar oscilador interno
//******************************************************************************
void initOscilador(char frec){
    
    switch(frec){
        case 0: 
            // Frecuencia de 31KHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
        case 1: 
            // Frecuencia de 125KHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            break;
        case 2: 
            // Frecuencia de 250KHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;
            break; 
        case 3: 
            // Frecuencia de 500KHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;
            break;
        case 4: 
            // Frecuencia de 1MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 5: 
            // Frecuencia de 2MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            break;
        case 6: 
            // Frecuencia de 4MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break; 
        case 7: 
            // Frecuencia de 8MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
        default:
            // Frecuencia de 8MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
    
    }
    OSCCONbits.SCS = 1;         //Inicia con el oscilador interno
}