#ifndef INT_QUEUE_TIMER_H
#define INT_QUEUE_TIMER_H

#include "globals.h"

void vInitialiseTimers( my_bool verbose );
void TIMER0_IRQHandler( void );
void TIMER1_IRQHandler( void );
void TIMER2_IRQHandler( void );

#endif
