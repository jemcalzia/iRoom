
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
#include <xc.h>
#include <stdint.h>
#include "I2C.h"
#include "Oscilator.h" 
#include "timer1.h"

uint8_t z, pir, min, hr, alarm, read, flag;
uint16_t timer; 

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
            if (read == 0) {
                min = SSPBUF; // Guardar en el PORTD el valor del buffer de recepción
                read++;
            }
            if (read == 1) {
                hr = SSPBUF;
                read = 0;
            }
            __delay_us(250);

        } else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW) {
            z = SSPBUF;
            BF = 0;
            SSPBUF = pir;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while (SSPSTATbits.BF);

        }

        PIR1bits.SSPIF = 0;
    }
    if (PIR1bits.TMR1IF) {
        t1_count++;
        TMR1 = OFFSET; // Se reinicia el timer 1 
        PIR1bits.TMR1IF = 0;
    }
    }

void setup()
    {
    oscilator_begin(7);
    I2C_Slave_Init(4);
    ANSEL = 0;
    TRISA0 = 1;
    TRISA2 = 0;
    TRISA1 = 0;
    timer1_begin(0, 3);
    read = 0;
    }

void main(void)
    {
    setup();
    pir = RA0;
    while (1) {
        timer ++;
                pir = RA0;
        if (pir == 1) {

            RA2 = 1;
            flag = 1;
            t1_count = 0;
        }
        if (flag == 1) {
            if (t1_count > 240) {
                RA2 = 0;
                flag = 0;
            }
        }


        if (hr == 0 & min == 0 & alarm == 0) {
            alarm = 1;
            timer = 0;
            hr++;
            RA1 = 1;
        }
        if (timer == 65535) {
            RA1 = 0;
            alarm = 0;
        }
    }
    }
