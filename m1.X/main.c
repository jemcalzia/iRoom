

// 'C' source line config statements

// PIC16F887 Configuration Bit Settings
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
#define RS RA0
#define EN RA1
#define tx PORTCbits.RC6
#define prueba PORTDbits.RD2
#define Signal PORTDbits.RD7
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
unsigned char minuto, hora, hr, ap, segundo, cual_dato;
unsigned char segundoDer, segundoIzq, minutoDer, minutoIzq, horaDer, horaIzq;
uint8_t puerta, luz,pir,hum,temperatura,counter;
char show[20];
//variables temporales
unsigned char humedad = 62U;
unsigned char temperatura = 27U;


void setup(void)
    {
    oscilator_begin(7);
    I2C_Master_Init(100000);
    
    TRISA = 0; 
    TRISB = 0;
    TRISD = 0;
    TRISCbits.TRISC7 = 1;       //Entrada RX
    ANSEL = 0;
    ANSELH = 0;
    PORTA = 0; 
    PORTB = 0;
    prueba = 0;
    
    LCD8_begin();
    LCD8_clear();
    timer1_begin(0,3);
    
    // USART TX & RX:
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.BRGH = 1;         //BRG16 = 0 & BRGH = 1
    SPBRGH = 0x0;
    SPBRGH = 0x51;              //n = 51 =>Baud = 9600 a 8 MHz
    TXSTAbits.SYNC = 0;         //asincrono
    RCSTAbits.SPEN = 1;         //habilitar puerto serial -> TXpin = Output
    TXSTAbits.TX9 = 0;          //9-bits NO
    TXSTAbits.TXEN = 1;
    CREN = 1;                   //Recibe datos en RX
    
    
    
   
    write_ds1307(0, 0U); //segundos
    write_ds1307(1, 1U); //minutos
    write_ds1307(2, 4U); //hora
  
    }

//Funcion de envío de datos por TX.
void UART_Tx(unsigned char val)
{
    while(TXIF == 0);    // Wait till the transmitter register becomes empty
    TXIF = 0;            // Clear transmitter flag
    TXREG = val;          // load the char to be transmitted into transmit reg
    
}

//Funcion para recivir datos en RX
unsigned char UART_Rx()
{
    while(RCIF == 0);    // Wait till the data is received 
    RCIF = 0;            // Clear receiver flag
    return(RCREG);     // Return the received data to calling function
}

void main(void)
    {
    setup();
    //prueba = 1;
    while (1){
        prueba =0;
        
        counter++;
        //Slave 1: Obstaculo + Luz
            I2C_Master_Start();
            I2C_Master_Write(0x20);
            I2C_Master_Write(0x1); 
            I2C_Master_Stop();
            __delay_ms(10);
            
            
            
            I2C_Master_Start();
            I2C_Master_Write(0x21);
            puerta = I2C_Master_Read(0);
            I2C_Master_Stop();
           __delay_ms(10);
            
    
            
            I2C_Master_Start();
            I2C_Master_Write(0x20);
            I2C_Master_Write(0x2);
            I2C_Master_Stop();
            __delay_ms(10);
            
           
            
            I2C_Master_Start();
            I2C_Master_Write(0x21);
            luz = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);
            
            
            
        //Slave 2: PIR + hora
            //Llamar Esclavo SENSOR TIEMPO REAL para recibir datos.
            segundo = read_DS1307(0); //obtener segundos
            __delay_ms(10);
            minuto = read_DS1307(1); //obtener minutos
            __delay_ms(10);
            hora = read_DS1307(2); //obtener hora
            __delay_ms(10);
            hr = hora & 0b00011111; //obtener hora
            __delay_ms(10);
            ap = hora & 0b00100000; //obtener 'AM' o 'PM
            __delay_ms(10);
            
         
            
            I2C_Master_Start();
            I2C_Master_Write(0x30);
            I2C_Master_Write(minuto);
            I2C_Master_Stop();
            __delay_ms(10);
            
           
            
            I2C_Master_Start();
            I2C_Master_Write(0x31);
            pir = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);
            
            
            /*
            sprintf(show,"%d",pir);
            LCD8_set_cursor(1,1);
            LCD8_strWrite(show);*/
            
            I2C_Master_Start();
            I2C_Master_Write(0x30);
            I2C_Master_Write(hr); 
            I2C_Master_Stop();
            __delay_ms(10);
            
           
            /*
            LCD8_set_cursor(1,6);
            LCD8_strWrite("AA");
            __delay_ms(1000);
            */
        //Slave 3: humedad + temp
            I2C_Master_Start();
            I2C_Master_Write(0x40);
            I2C_Master_Write(0x1); 
            I2C_Master_Stop();
           __delay_ms(10);
            
            I2C_Master_Start();
            I2C_Master_Write(0x41);
            hum = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);
            
            
            
            I2C_Master_Start();
            I2C_Master_Write(0x40);
            I2C_Master_Write(0x2);
            I2C_Master_Stop();
            __delay_ms(10);
              
            
            I2C_Master_Start();
            I2C_Master_Write(0x41);
            temperatura = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);
       
           /* if (t1_count<150){
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
                */
           
           
            //UART:
            Signal = 1;
            cual_dato = UART_Rx();
         prueba = 1;
        if(cual_dato == 1U){
            UART_Tx(hum);//cambiar
        }else if(cual_dato == 2U){
            UART_Tx(puerta);
        }else if(cual_dato == 3U){
            UART_Tx(temperatura);//cambiar
        }else if(cual_dato == 4U){
            UART_Tx(pir);
        }else if(cual_dato == 5U){
            UART_Tx(hr);
        }else if(cual_dato == 6U){
            UART_Tx(minuto);
        }else if(cual_dato == 7U){
            UART_Tx(luz);
        }
            Signal = 0;
            
            LCD8_clear();
            if(t1_count<177){
                LCD8_set_cursor(1,1);
            LCD8_strWrite("Luz");
            if(luz<100){
            sprintf(show,"%d ",luz);
            LCD8_set_cursor(2,1);
            LCD8_strWrite(show);
            }
            
            if(luz>99){
            sprintf(show,"%d",luz);
            LCD8_set_cursor(2,1);
            LCD8_strWrite(show);
            }
        
            
            LCD8_set_cursor(1,6);
            LCD8_strWrite("Dist.");
             if(puerta <100){
            sprintf(show,"%dcm ",puerta);
            LCD8_set_cursor(2,6);
            LCD8_strWrite(show);
            }
            
            if(puerta>99){
            sprintf(show,"%dcm",puerta);
            LCD8_set_cursor(2,6);
            LCD8_strWrite(show);
            }
            
            LCD8_set_cursor(1,12);
            LCD8_strWrite("Mov");
            LCD8_set_cursor(2,12);
            if (pir == 1){      
            LCD8_strWrite("Si");
            }
            else if (pir == 0){      
            LCD8_strWrite("No");
            } else{
               LCD8_strWrite("N/A"); 
            }
            }
            if(t1_count>=177){
                
            LCD8_set_cursor(1,1);
            LCD8_strWrite("Hora");
            sprintf (show,"%d",hr);
            LCD8_set_cursor(2,1);
            LCD8_strWrite(show);
            sprintf (show,":%d",minuto);
            LCD8_set_cursor(2,2);
            LCD8_strWrite(show);
            
            LCD8_set_cursor(1,7);
            LCD8_strWrite("Hum + Temp");
            if(hum <100){
            sprintf(show,"%d ",hum);
            LCD8_set_cursor(2,7);
            LCD8_strWrite(show);
            }
            if(hum>99){
            sprintf(show,"%d",hum);
            LCD8_set_cursor(2,7);
            LCD8_strWrite(show);
            }
            
            if(temperatura <100){
            sprintf(show,"%d C",temperatura);
            LCD8_set_cursor(2,13);
            LCD8_strWrite(show);
            }
            if(temperatura>99){
            sprintf(show,"%dC",temperatura);
            LCD8_set_cursor(2,13);
            LCD8_strWrite(show);
            }
           
            
            }
            
            
    }
}
