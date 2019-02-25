
#ifndef STEPPER_H
#define	STEPPER_H


#include <stdint.h>
void full_step(uint16_t steps);
void wave_step(uint16_t steps); 
void full_rev(uint16_t steps);
void wave_rev(uint16_t steps); 

#endif	/* STEPPER_H */

