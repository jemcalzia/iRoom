/* 
 * File:   OsciladorLibs.h
 * Author: Jose Molina 
 * Libreria para controlar el oscilador interno del PIC16F887   
 * Created on 15 de enero de 2019, 11:07 AM
 */

//Funcion para inicializar 
void oscilator_begin(char freq){
    OSCCONbits.IRCF = freq;
    OSCCONbits.SCS = 1;
    OSCCONbits.OSTS = 0;
}