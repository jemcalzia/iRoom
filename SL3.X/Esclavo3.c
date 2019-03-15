
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 8000000
#define prueba PORTAbits.RA1
#include <xc.h>
#include <stdint.h>
#include "Oscilator.h"
#include "stepper.h"
#include "dht11_h.h"
#include "timer1.h"
#include "I2C.h"

uint8_t temp_int, temp_dec, hum_int, hum_dec, check, total, z, var, state,count;

void __interrupt() isr(void)
    {
    if (PIR1bits.SSPIF == 1) {
 
        SSPCONbits.CKP = 0;

        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)) {
            z = SSPBUF; // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0; // Clear the overflow flag
            SSPCONbits.WCOL = 0; // Clear the collision bit
            SSPCONbits.CKP = 1; // Enables SCL (Clock)
        }

        if (!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF; // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0; // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1; // Habilita entrada de pulsos de reloj SCL
            while (!SSPSTATbits.BF); // Esperar a que la recepción se complete
            var = SSPBUF;
            //prueba = 1;
            __delay_us(250);


        }
        if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) {
prueba = 1;
            z = SSPBUF;
            BF = 0;
            if (var == 1) {
                SSPBUF = hum_int;
                
            }
            if (var == 2) {
                SSPBUF = temp_int;
            }

            //prueba =1; 
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while (SSPSTATbits.BF);
           
        }

        PIR1bits.SSPIF = 0;
        
    }
    if (PIR1bits.TMR1IF) {
        t1_count++;
        TMR1 = OFFSET; // Se reinicia el timer 1 
        
        if (t1_count == 19) {
           
           dht11_begin();
            dht11_check();
            hum_int = dht11_read();
            hum_dec = dht11_read();
            temp_int = dht11_read();
            temp_dec = dht11_read();
            check = dht11_read();
            total = hum_int + hum_dec + temp_int + temp_dec;
           
        }
        
        
        
        PIR1bits.TMR1IF = 0;
    }
    }

void main(void)
    {
    oscilator_begin(7);
    TRISD = 0;
    PORTD = 0;
    TRISA1 = 0;
    ANSEL = 0;
    state = 0;
    TRISE = 0; 
    PORTE = 0; 

    
    prueba = 0;

    timer1_begin(OFFSET, PRESCALER);
    I2C_Slave_Init(0x40);
    PORTD = 0;
    while (1) {
      
        if (hum_int > 85) {
            if (state == 0) {
                full_rev(1605U);
                state = 1U;
                PORTD = 0;
            } else {
                PORTD = 0;
            }
            
        }
        if (hum_int <= 85 && state) {
            full_step(1605U);
            state = 0;
            PORTD = 0;
        }
        
        if (temp_int > 25) {
            RE0 = 1;
        } else {
            RE0 = 0;
        }
    }
    }
