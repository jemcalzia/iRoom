/*
 Sensor Obstaculo --> Servo para puerta
 Sensor Luz --> Intensidad de luz
 */

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

// --- CONEXIONES ---
#define _XTAL_FREQ 8000000

#define foco PORTBbits.RB5                                  
#define servo PORTCbits.RC1

// --- LIBRERIAS ---
#include <xc.h>
#include "I2C.h"
#include "Oscilador.h"
#include "Timer0.h"

// --- VARIABLES ---
short z;
short dato;
unsigned int a;
unsigned char variable, ADC_LUZ, on_time, duty_cycle, ADCLUZI2C;
unsigned char pres = 131U;
unsigned char contPWM = 0U;
unsigned char contLuz = 0U;
unsigned char puerta = 0U;      //puerta abierta o cerrada (0 = abierta, 1 = cerrada)
char flag,door,state; 

// --- INTERRUPCIONES ---
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        // --LECTURA --:
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            variable = SSPBUF;          
            __delay_us(250);
            
            // --ESCRITURA--:
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){    
            z = SSPBUF;
            BF = 0;
            if(variable == 0x1){
              SSPBUF = ADCLUZI2C;  
            }else if(variable == 0x2){
               SSPBUF = a; 
            }
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   
   if(TMR0IF == 1)        //Timer0
    {
        TMR0 = 132;     //retraso de 1 para arrancar TMR0
        TMR0IF=0;       
        contPWM++;
        contLuz++;
        
        //Intensidad de Luz del foco:
        /*
         Elperiodo dura 5ms = 10*0.5ms 
         */
        if(contLuz < duty_cycle){
            foco = 1;                   //Tiempo HIGH
        }else{
            foco = 0;                   //Tiempo LOW
            if(contLuz == 10U){         //termina periodo y reinicializamos
                contLuz = 0U;
            }
        }
        
    if (contPWM <= on_time) //tiempo en HIGH
    {
        servo = 1;  
    }
    
    if (contPWM >= on_time)    //tiempo en LOW
    {   
        servo = 0;
        if(contPWM == 40U){    //Se reinicializa el periodo de onda => 
                            //40*0.5ms = 20ms = periodo de onda
            contPWM = 0U;
        }
    }
  } 
}

// --- FUNCIONES ---
void setup(void){
    initOscilador(7);           // Oscilador interno de 8 MHz
    ANSEL = 0;
    ANSELH = 0;
    TRISD = 0b00000010; // RD1 COMO ENTRADA (ECHO) Y RD0 COMO SALIDA (TRIGGER)
    TRISB = 0;

    PORTA = 0;
    PORTD = 0;
    TRISB = 0;
    TRISAbits.TRISA0 = 1;       //Luz     
    TRISAbits.TRISA4 = 1;       //Obstaculo
    TRISCbits.TRISC1 = 0;       //Servo
    
    foco = 0;
    servo = 0;
    on_time = 0x2;              //2*0.5ms = 1ms --> 0 grados; puerta normalmente abierta
    duty_cycle = 0U;
    
    //Timer0
    initTimero(0, 0, 2, pres); //temporizador, 1:8 con 131 de precargado para 0.5ms.
    
    //Timer1
    T1CON = 0x10;               // INICIALIZO EL TIMER
    
    //ADC
    ANSELbits.ANS0 = 1;         //RA0 --> ADC
    ADCON1bits.VCFG0 = 0;       //Vss y Vdd como referencia
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.ADFM = 0;        //justificado a izquierda
    ADCON0bits.ADCS1 = 1;       //fosc/32
    ADCON0bits.ADCS0 = 0;
    ADCON0bits.ADON = 1;        //inicializar ADC
    ADCON0bits.CHS3 = 0;        //ADC en ANS0->RA0
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;
    __delay_ms(2);              //Tiempo de carga capacitores
    
    //I2C
    I2C_Slave_Init(0x02);       //Inicialización de esclavo con dirección 0x20
}

//Funcion para leer el valor del ADC:
unsigned char read_ADC(){
  ADCON0bits.GO_nDONE = 1;          //Inicializar conversion ADC
  while(ADCON0bits.GO_nDONE == 1);  //Esperar finalizacion de conversion
  return (ADRESH);     
}

//Funcion para determinarl el valor del Duty Cycle:
/* ADC_LUZ envía valores de 0U (para mucha luz entrante
 a 150U (para auscencia de luz completa). Para poca luz
 la intensidad del foco es mayor y para mucha luz entrante
 la intensidad de luz es tenue.
 */
unsigned char DutyCycle(unsigned char ADC_LUZ){
        if (ADC_LUZ < 13U){
            duty_cycle = 0U;
        }else if((ADC_LUZ > 13U) && (ADC_LUZ < 26U)){
            duty_cycle = 1U;
        }else if((ADC_LUZ > 26U) && (ADC_LUZ < 39U)){
            duty_cycle = 2U;
        }else if((ADC_LUZ > 39U) && (ADC_LUZ < 52U)){
            duty_cycle = 3U;
        }else if((ADC_LUZ > 52U) && (ADC_LUZ < 65U)){
            duty_cycle = 4U;
        }else if((ADC_LUZ > 65U) && (ADC_LUZ < 78U)){
            duty_cycle = 5U;
        }else if((ADC_LUZ > 78U) && (ADC_LUZ < 91U)){
            duty_cycle = 6U;
        }else if((ADC_LUZ > 91U) && (ADC_LUZ < 104U)){
            duty_cycle = 7U;
        }else if((ADC_LUZ > 104U) && (ADC_LUZ < 117U)){
            duty_cycle = 8U;
        }else if((ADC_LUZ > 117U) && (ADC_LUZ < 130U)){
            duty_cycle = 9U;
        }else if((ADC_LUZ > 130U) && (ADC_LUZ < 143U)){
            duty_cycle = 10U;
        }
        return(duty_cycle);
}

void main(void) {
    setup();
    flag = 0;
    state = 0; 

    while(1){
        flag ++;
        if (flag == 255){
            door ++;
        }
        ADC_LUZ = read_ADC();             //leer ADC
        ADCLUZI2C = ADC_LUZ;              //variable que se envía en I2C
        
        duty_cycle = DutyCycle(ADC_LUZ); //determinar valor del duty cycle del foco
        
        //SERVO:
        /*
        * El servo funciona siempre con un periodo de 20ms.
        * Se dirige a 90 grados con un HIGH de 1.5ms.
        * Se dirige a 0 grados con un HIGH de 1ms.
        * ¡ver FuncServoPWM.jpg! 
        */
        if(door==100){
            door = 0; 
        //OBSTACULO:
        TMR1H = 0;
        TMR1L = 0;                      // EL VALOR DEL TIMER = 0
        PORTDbits.RD0 = 1;              // TRIGGER EN HIGH
        __delay_us(10);
        PORTDbits.RD0 = 0;              // TRIGGER EN LOW
        
        while(!PORTDbits.RD1);          //SE ESPERA EL ECHO
        TMR1ON = 1;                     //INICIA EL TIMER
        while(PORTDbits.RD1);           // ESPERO HASTA EL EL ECHO SE APAGUE
        TMR1ON = 0;                     // DETENGO EL TIMER

        a = (TMR1L | (TMR1H<<8));       // LEO EL VALOR DEL TIMER
        a = a/58.82;                    // CONVIERTO EL TIEMPO A DISTANCIA
        a = a + 1;                      // CALIBRACION DE LA MEDICION

        if (a<=7)
        {
            //puerta = ~puerta;            //se abre o se cierra puerta
            state = 1; 
        }
        else{
            state = 0; 
        }
        
        //Valor para servo paraabrir o cerrar puerta
        if (state == 1){
            on_time = 0x2;                 //2*0.5ms = 1ms --> 0 grados para abrir puerta 
        }
        if(state == 0){   
            on_time = 0x3;              //3*0.5ms = 1.5ms --> 90 grados para cerrar puerta
        }
        
        
        }
    }
    return;
}