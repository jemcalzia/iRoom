
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
#define RS RA1
#define EN RA2

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "dht11_h.h"
#include "LCD8bit.h"
#include "Oscilator.h"
#include "timer1.h"
 

uint8_t temp_int, temp_dec, hum_int, hum_dec, check, total,counter;
char show[15];

void main(void)
    {
    ANSEL = 0; 
    TRISA1 = 0; 
    TRISA2 = 0; 
    TRISA3 = 0; 
    PORTA = 0; 
    PORTB = 0; 
    TRISB = 0; 
    oscilator_begin(7);
    LCD8_begin();
    LCD8_clear();
    LCD8_set_cursor(1,1);
    LCD8_strWrite("Funciona");
    __delay_ms(100);
    timer1_begin(OFFSET,PRESCALER); 
    
   while(1)
    {
       LCD8_clear();
       if(t1_count == 10)
       {
       
       dht11_begin();
       dht11_check();
       hum_int = dht11_read();
       hum_dec = dht11_read();
       temp_int = dht11_read();
       temp_dec = dht11_read();
       check = dht11_read();
       total = hum_int+hum_dec+temp_int+temp_dec;
       
       if (check != total){
           LCD8_clear();
           LCD8_set_cursor(1,1);
           LCD8_strWrite("Error, revisar");
           LCD8_set_cursor(2,1);
           LCD8_strWrite("comunicacion");
       }
       t1_count = 0; 
       }
       LCD8_set_cursor(1,1);
       sprintf(show, "Humedad: %d",hum_int);
       LCD8_strWrite(show);
       sprintf(show, ".%d%",hum_dec);
       LCD8_strWrite(show);
       
       LCD8_set_cursor(2,1);
       sprintf(show, "Temp: %d",temp_int);
       LCD8_strWrite(show);
       sprintf(show, ".%dC",temp_dec);
       LCD8_strWrite(show);
       
       LCD8_set_cursor(2,14);
       sprintf(show, "%d",t1_count);
       LCD8_strWrite(show);
    }
    
    }
