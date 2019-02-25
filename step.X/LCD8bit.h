// Se deben de defir EN y RS en algun puerto con el siguiente comando: 
// #define RS PORTDbits.RD0
// Esta librería funcionará con el puerto B entero RB0:RB7
void LCD8_write(char data){ // Format: 'a'
    RS = 1;
    PORTB = data;
    EN = 1;
    __delay_us(40);
    EN = 0;   
    
}
void LCD8_cmd(char data){
    RS = 0; // Register Select 
    PORTB = data; //D0:D7 -> RB0:RB7 
    
    EN = 1; //Enable HIGH
    __delay_ms(4); //Read on change
    EN = 0;  //Enable LOW
}

void LCD8_set_cursor(char row, char column){
    char cursor; 
    // registro 128 y 192 
        if(row==1)
            cursor = 128 + column -1;
        if (row==2)
            cursor = 192 + column -1;
    
        LCD8_cmd(cursor);  
}

void LCD8_clear(void){
    LCD8_cmd(0);
    LCD8_cmd(1);
}

void LCD8_begin(void){
    PORTB = 0; 
    __delay_ms(20);
    LCD8_cmd(48);   
    __delay_ms(5);
    LCD8_cmd(48);
    __delay_us(165);
    LCD8_cmd(48);
    __delay_us(165);
    
    LCD8_cmd(0b111000);                    
    LCD8_cmd(0b10100);                      
    LCD8_cmd(1);                           
    LCD8_cmd(0b110);                      
    LCD8_cmd(0b1100);                       
    LCD8_cmd(0b10);
}

void LCD8_strWrite(char *data){
    char i;
        for(i=0; data[i] !='\0';i++ )
            LCD8_write(data[i]); 
}

void LCD8_shiftR() {
    LCD8_cmd(0x01);
    LCD8_cmd(0x0C);
}

void LCD8_shiftL() {
    LCD8_cmd(0x01);
    LCD8_cmd(0x08);
}
/* Example for LCD8_shiftR & LCD4_shifrL
     * for(a=0;a<15;a++)
    {
        __delay_ms(300);
        LCD8_shiftR();
    }*/