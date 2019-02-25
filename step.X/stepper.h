
#ifndef STEPPER_H
#define	STEPPER_H
#define HERZ_100 (10)
#define HERZ_50 (20)
#define HERZ_10 (10)
#define HERZ_200 (5)


void full_step(uint8_t speed, uint16_t steps);
void wave_step(uint8_t speed, uint16_t steps); 


#endif	/* STEPPER_H */

