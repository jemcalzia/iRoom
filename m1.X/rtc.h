//***************************

//                 SENSOR DE TIEMPO REAL DS1307

/* 
 * File:                DS1307.h
 * Author:              JOnathan Saenz SAE16263
 * Descripcion:         Librería para el sensor de reloj en tiempo real DS1307
 */

/* 
 --- NOTA: Para esta librería se tomó como base el programa elaborado en 
 * https://www.studentcompanion.co.za/interfacing-the-ds1307-real-time-clock-with-pic-microcontroller-mikroc/
 */

//Funcion para leer datos del SENSOR DS1307 
unsigned short read_DS1307(unsigned short direccion)
{
    unsigned short tiempo;
    I2C_Master_Start();
    I2C_Master_Write(0xD0);         
    I2C_Master_Write(direccion);    //decirle qué tipo de dato queremos obtener
    I2C_Master_RepeatedStart();     //otro Start
    I2C_Master_Write(0xD1); 
    tiempo = I2C_Master_Read(0);
    I2C_Master_Stop();
    return(tiempo);
}

void write_ds1307(short address, short w_data)
{
  I2C_Master_Start(); // issue I2C start signal
  //address 0x68 followed by direction bit (0 for write, 1 for read) 0x68 followed by 0 --> 0xD0
  I2C_Master_Write(0xD0); // send byte via I2C (device address + W)
  I2C_Master_Write(address); // send byte (address of DS1307 location)
  I2C_Master_Write(w_data); // send data (data to be written)
  I2C_Master_Stop(); // issue I2C stop signal
}

//Extraer de nibble izquierdo de BCD
char BCD2UpperCh(char bcd)
{
  return (bcd >> 4);
}

//Extraer de nibble izquierdo de BCD
char BCD2LowerCh(char bcd)
{
  return (bcd & 0x0F);
}

//Binario a BCD
int Binary2BCD(int a)
{
   int t1, t2;
   t1 = a%10;
   t1 = t1 & 0x0F;
   a = a/10;
   t2 = a%10;
   t2 = 0x0F & t2;
   t2 = t2 << 4;
   t2 = 0xF0 & t2;
   t1 = t1 | t2;
   return t1;
}

//BCD a binario
int BCD2Binary(int a)
{
   int r,t;
   t = a & 0x0F;
   r = t;
   a = 0xF0 & a;
   t = a >> 4;
   t = 0x0F & t;
   r = t*10 + r;
   return r;
}