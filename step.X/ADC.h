
void ADC_option(char option) {
    switch (option) {
        case 0: //RA0
            ANSELbits.ANS0 = 1;
            TRISAbits.TRISA0 = 1;
            break;
        case 1: //RA1
            ANSELbits.ANS1 = 1;
            TRISAbits.TRISA1 = 1;
            break;
        case 2: //RA2
            ANSELbits.ANS2 = 1;
            TRISAbits.TRISA2 = 1;
            break;
        case 3: //RA3
            ANSELbits.ANS3 = 1;
            TRISAbits.TRISA3 = 1;
            break;
        case 4: //RA5
            ANSELbits.ANS4 = 1;
            TRISAbits.TRISA5 = 1;
            break;
        case 5: //RE0
            ANSELbits.ANS5 = 1;
            TRISEbits.TRISE0 = 1;
            break;
        case 6: //RE1
            ANSELbits.ANS6 = 1;
            TRISEbits.TRISE1 = 1;
            break;
        case 7: //RE2
            ANSELbits.ANS7 = 1;
            TRISEbits.TRISE2 = 1;
            break;
        case 8: //RB2
            ANSELHbits.ANS8 = 1;
            TRISBbits.TRISB2 = 1;
            break;
        case 9: //RB3
            ANSELHbits.ANS9 = 1;
            TRISBbits.TRISB3 = 1;
            break;
        case 10: //RB1
            ANSELHbits.ANS10 = 1;
            TRISBbits.TRISB1 = 1;
            break;
        case 11: //RB4
            ANSELHbits.ANS11 = 1;
            TRISBbits.TRISB4 = 1;
            break;
        case 12: //RB0
            ANSELHbits.ANS12 = 1;
            TRISBbits.TRISB0 = 1;
            break;
        case 13: //RB5
            ANSELHbits.ANS13 = 1;
            TRISBbits.TRISB5 = 1;
            break;
        default: //RA0
            ANSELbits.ANS0 = 1;
            TRISAbits.TRISA0 = 1;
            break;
    }
}

void ADC_freq(char freq) {
    /*
     * freq = 0: Use only with Fosc = 1MHz
     * freq = 1: Use only with Fosc = 4Mhz
     * freq = 2: Use with 20MHz or 8MHz
     * freq = 3: FRC function, use only when Sleep Mode. 
     */
    ADCON0bits.ADCS = freq;
}

void ADC_channel(char channel) {
    /*
     * Analog channel select 0-13
     */
    ADCON0bits.CHS = channel;
}

void ADC_begin(char option, char freq) {
    /*
     * Initalize ADC module 
     */
    ADC_option(option);
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.ADFM = 0;
    ADCON0bits.ADON = 1; 
    /*PIE1bits.ADIE = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;*/
    ADC_freq(freq);
    //PIR1bits.ADIF = 1;
    ADC_channel(option);
}

void ADC_int_begin(char option, char freq) {
    /*
     * Initalize ADC module 
     */
    ADC_option(option);
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.ADFM = 0;
    PIE1bits.ADIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    ADC_freq(freq);
    PIR1bits.ADIF = 1;
    ADC_channel(option);
}
    
char ADC_conversion(void) {
    ADCON0bits.GO_nDONE = 1; 
    while(ADCON0bits.GO_nDONE);
    return ADRESH; 
    
}
void ADC_int_conversion(void) {
    ADCON0bits.ADON = 1;
    __delay_ms(50);
    ADCON0bits.GO_DONE = 1;
}