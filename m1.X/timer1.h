/* 
 * File:   timer17.h
 * Author: jmoli
 *
 * Created on 24 de febrero de 2019, 02:00 PM
 */

#include <stdint.h>
#ifndef TIMER1_H
#define	TIMER1_H

#define OFFSET (0)
#define PRESCALER (2)
uint8_t t1_count; 
void timer1_begin(uint8_t offset, uint8_t prescaler);

#endif	/* TIMER17_H */

