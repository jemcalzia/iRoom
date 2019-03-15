/* 
 * File:                Libreria Configuracion Oscilador
 * Author:              Jonathan Saenz SAE16263
 * Comments:            Configurar todos los bits del oscilador
 */


void initOsc(char frec, char osts, char hts, char lts, char scs){

    switch(frec){
        case 0:
            //frecuencia de 31 KHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;
            
            break;
        case 1:
            //frecuencia de 125 KHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 0;

            break;
        case 2:
            //frecuencia de 250 KHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;

            break;
        case 3:
            //frecuencia de 500 KHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 0;

            break;
        case 4:
            //frecuencia de 1 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;
            
            break;
        case 5:
            //frecuencia de 2 MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 0;
            OSCCONbits.IRCF2 = 1;

            break;
        case 6:
            //frecuencia de 4 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;

            break;
        case 7:
            //frecuencia de 8 MHz
            OSCCONbits.IRCF0 = 1;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;

            break;
        default:
            //frecuencia de 4 MHz
            OSCCONbits.IRCF0 = 0;
            OSCCONbits.IRCF1 = 1;
            OSCCONbits.IRCF2 = 1;
            break;
    }
    if(osts==1){
        OSCCONbits.OSTS = 1;
    } else{
        OSCCONbits.OSTS = 0;
    }
    
    if(hts==1){
        OSCCONbits.HTS = 1;
    } else{
        OSCCONbits.HTS = 0;
    }
    
    if(lts==1){
        OSCCONbits.LTS = 1;
    } else{
        OSCCONbits.LTS = 0;
    }
    
    if(scs==1){
        OSCCONbits.SCS = 1;
    } else{
        OSCCONbits.SCS = 0;
    }
}