//*****************************************************************************
/*
 La Raspberry manda los datos a la nube.
 */
//*****************************************************************************
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
#include "oscilador.h"
#include "SPI.h"

#define prueba PORTDbits.RD2
#define prueba2 PORTDbits.RD3
#define Signal PORTAbits.RA0
char estado = 0;  //defino variables
char contador = 255;
char disp = 0;

unsigned char precensia, hora, minutos, distancia, temperatura, humedad,luz;

void setup(void);

void __interrupt() isr(void){

   if(SSPIF == 1){
        
       estado = spiRead();
       prueba2 =1;
       if (estado == 0x1)
       {
           spiWrite(humedad);
       }else if (estado == 0x2)
       {
            spiWrite(precensia);   
       }else if (estado == 0x3)
       {
            spiWrite(temperatura);   
       }else if (estado == 0x4)
       {
            spiWrite(distancia);   
       }
       else if (estado == 0x5)
       {
            spiWrite(hora);   
       }else if (estado == 0x6)
       {
            spiWrite(minutos);   
       }else if (estado == 0x7)
       {
            spiWrite(luz);   
       }
        
       SSPIF = 0; 
       
   }
   
}

//Funcion de envío de datos por TX.
void UART_Tx(unsigned int val)
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

    while(1){
        
        
        while(Signal){
        UART_Tx(1U);
        prueba = 1;
        humedad = UART_Rx();
        __delay_ms(10);
        UART_Tx(2U);
        precensia = UART_Rx();
        __delay_ms(10);
        UART_Tx(3U);
        temperatura = UART_Rx();
        __delay_ms(10);
        UART_Tx(4U);
        distancia = UART_Rx();
        __delay_ms(10);
        UART_Tx(5U);
        hora = UART_Rx();
        __delay_ms(10);
        UART_Tx(6U);
        minutos = UART_Rx();
        __delay_ms(10);
        UART_Tx(7U);
        luz = UART_Rx();
        __delay_ms(10);
        
        }
        
        
    }

}
void setup(void){
    initOsc(7, 1, 1, 0 ,1);     //Oscilador interno 8 MHz
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 1U;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    TRISCbits.TRISC7 = 1;       //Entrada RX
    
    prueba = 0;
    prueba2 = 0;

    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupcion SPI
    PIE1bits.SSPIE = 1;         // Habilitamos interrupcion SPI
    TRISAbits.TRISA5 = 1;       // Slave Select
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_ACTIVE_2_IDLE);
    
    // USART TX & RX:
    BAUDCTLbits.BRG16 = 0;
    TXSTAbits.BRGH = 1;         //BRG16 = 0 & BRGH = 1
    SPBRGH = 0x0;
    SPBRGH = 0x51;              //n = 51 =>Baud = 9600 a 8 MHz
    TXSTAbits.SYNC = 0;         //asincrono
    RCSTAbits.SPEN = 1;         //habilitar puerto serial -> TXpin = Output
    TXSTAbits.TX9 = 0;          //9-bits NO
    TXSTAbits.TXEN = 1;
    CREN = 1;

}