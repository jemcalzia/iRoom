/* 
 * File:                Libreria Timer0
 * Author:              Jonathan Saenz 16263
 * Descripcion:         Inicializacion del Timer 0.
 */

void initTimero (int tocs, int psa, int prescaler, int valor){
    
    if(tocs==1){
        OPTION_REGbits.T0CS = 1;
    }else{
        OPTION_REGbits.T0CS = 0;
    }
   
    if(psa == 1){
        OPTION_REGbits.PSA = 1;
    }else{
        OPTION_REGbits.PSA = 0;
    }
    
    switch(prescaler){
        case 0:
            //1:2
            OPTION_REGbits.PS0 = 0;
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS2 = 0;
            break;
        case 1:
            //1:4
            OPTION_REGbits.PS0 = 1;
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS2 = 0;
            break;
        case 2:
            //1:8
            OPTION_REGbits.PS0 = 0;
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS2 = 0;
            break;
        case 3:
            //1:16
            OPTION_REGbits.PS0 = 1;
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS2 = 0;
            break;
        case 4:
            //1:32
            OPTION_REGbits.PS0 = 0;
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS2 = 1;
            break;
        case 5:
            //1:64
            OPTION_REGbits.PS0 = 1;
            OPTION_REGbits.PS1 = 0;
            OPTION_REGbits.PS2 = 1;
            break;
        case 6:
            //1:128
            OPTION_REGbits.PS0 = 0;
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS2 = 1;
            break;
        case 7:
            //1:256
            OPTION_REGbits.PS0 = 1;
            OPTION_REGbits.PS1 = 1;
            OPTION_REGbits.PS2 = 1;
            break;
    }
    
    INTCONbits.GIE = 1;         //Interrupcion global
    OPTION_REGbits.nRBPU = 1;
    INTCONbits.T0IE = 1;        //Inicializar Interrupcion
    INTCONbits.T0IF = 0;        //Borrar bandera
    TMR0 = valor;
}
