#include <xc.h>
#include <stdint.h>
#define DATA_DIR TRISA0
#define DATA PORTAbits.RA0
//Functions: 
void dht11_begin(void);
void dht11_check(void);
uint8_t dht11_read(void);



