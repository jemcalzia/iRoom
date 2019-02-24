/* 
 * File:   dht11_h.h
 * Author: jmoli
 *
 * Created on 23 de febrero de 2019, 06:35 PM
 */

#include <stdint.h>
#ifndef DHT11_H_H
#define	DHT11_H_H

#define DATA RA0
#define DIR TRISA0

//RA0 as 
//Functions: 
void dht11_begin(void);
void dht11_check(void);
uint8_t dht11_read(void);


#endif	/* DHT11_H_H */

