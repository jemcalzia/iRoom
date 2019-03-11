
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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define _XTAL_FREQ 8000000
#define SLAVE1READ 3
#define SLAVE1WRITE 2
#define SLAVE2READ 5
#define SLAVE2WRITE 4
#define SLAVE3READ 7 
#define SLAVE3WRITE 6
#define SLAVELCDREAD 9 
#define SLAVELCDWRITE 8
#define RS RA0
#define EN RA1
#include <xc.h>
#include "I2C.h"
#include "Oscilator.h"
#include "timer1.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "rtc.h"
#include "LCD8bit.h"


// --- VARIABLES ---
unsigned char variableADC, valor_CONT, a;
float convertido;
unsigned char decimal, apoyo_unitario, unitario;
unsigned char minuto, hora, hr, ap, segundo;
unsigned char segundoDer, segundoIzq, minutoDer, minutoIzq, horaDer, horaIzq;
unsigned char puerta, luz,pir,hum,temperatura;
char show[20];

void setup(void)
    {
    oscilator_begin(7);
    I2C_Master_Init(100000);
    
    TRISA = 0; 
    ANSEL = 0;
    
    LCD8_begin();
    
    timer1_begin(0,3);
    LCD8_set_cursor(1,1);
            LCD8_strWrite("Si pudo");
            __delay_ms(1000);
            write_ds1307(0, 0x0); //segundos
    write_ds1307(1, 0x6); //minutos
    write_ds1307(2, 0x11); //hora
    }

void main(void)
    {
    setup();
    while (1){
        LCD8_clear();
        //Slave 1: Obstaculo + Luz
            I2C_Master_Start();
            I2C_Master_Write(SLAVE1WRITE);
            I2C_Master_Write(1); 
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE1READ);
            puerta = I2C_Master_Read(0);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE1WRITE);
            I2C_Master_Write(2);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE1READ);
            luz = I2C_Master_Read(0);
            I2C_Master_Stop();
            
         
        //Slave 2: PIR + hora
            //Llamar Esclavo SENSOR TIEMPO REAL para recibir datos.
            segundo = read_DS1307(0); //obtener segundos
            minuto = read_DS1307(1); //obtener minutos
            hora = read_DS1307(2); //obtener hora
            hr = hora & 0b00011111; //obtener hora
            ap = hora & 0b00100000; //obtener 'AM' o 'PM
            I2C_Master_Start();
            I2C_Master_Write(SLAVE2READ);
            pir = I2C_Master_Read(0);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE2WRITE);
            I2C_Master_Write(minuto);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE2WRITE);
            I2C_Master_Write(hr); 
            I2C_Master_Stop();
            
        //Slave 3: humedad + temp
            I2C_Master_Start();
            I2C_Master_Write(SLAVE3WRITE);
            I2C_Master_Write(1); 
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE3READ);
            hum = I2C_Master_Read(0);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE3WRITE);
            I2C_Master_Write(2);
            I2C_Master_Stop();
            
            I2C_Master_Start();
            I2C_Master_Write(SLAVE3READ);
            temperatura = I2C_Master_Read(0);
            I2C_Master_Stop();
            
       
            if (t1_count<150){
                sprintf(show,"%d",hr);
                LCD8_set_cursor(2,1);
                LCD8_strWrite(show);
                sprintf(show,":%d",minuto);
                LCD8_strWrite(show);
                sprintf(show," H%d",hum);
                LCD8_strWrite(show);
                sprintf(show," T%d",temperatura);
                LCD8_strWrite(show);
            }
            if (t1_count > 150){
                
                sprintf(show,"D%d",puerta);
                LCD8_set_cursor(2,1);
                LCD8_strWrite(show);
                sprintf(show,"%d",luz);
                LCD8_strWrite(show);
                if(pir ==1){
                    sprintf(show,"SI");
                }
                else{
                    sprintf(show,"NO");
                }
                LCD8_strWrite(show);
                
            }
    }
}
