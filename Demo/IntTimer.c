/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Demo includes. */
#include "IntTimer.h"
/* Library includes. */
#include "CMSIS/S32K3X8EVB.h"
#include "uart.h"

#define tmrTIMER_0_FREQUENCY	( 2UL )
//#define tmrTIMER_1_FREQUENCY    ( 1UL )

void vInitialiseTimers( void ) {

    // Initialize TIMER0

    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;           // Clear any pending interrupts
    S32K3X8_TIMER0->RELOAD = ( configCPU_CLOCK_HZ / 
                               tmrTIMER_0_FREQUENCY );   // Set reload value
    S32K3X8_TIMER0->CTRL   = ( TIMER_CTRL_INTEN_Msk |    // Enable Timer interrupt
                               TIMER_CTRL_ENABLE_Msk );  // Enable Timer

    NVIC_SetPriority( TIMER0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER0_IRQn );

    // Initialize TIMER1

    /*
    S32K3X8_TIMER1->INTCLR = TIMER_INTCLR_Msk;           // Clear any pending interrupts
    S32K3X8_TIMER1->RELOAD = ( configCPU_CLOCK_HZ / 
                               tmrTIMER_1_FREQUENCY );   // Set reload value
    S32K3X8_TIMER1->CTRL   = ( TIMER_CTRL_INTEN_Msk |    // Enable Timer interrupt
                               TIMER_CTRL_ENABLE_Msk );  // Enable Timer

    NVIC_SetPriority( TIMER1_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER1_IRQn );
    */

}

void TIMER0_IRQHandler(void) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;

    UART_printf("Timer 0 Interrupt\n");

    /* Perform a context switch if necessary */
    //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

void TIMER1_IRQHandler(void) {

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    //S32K3X8_TIMER1->INTCLR = TIMER_INTCLR_Msk;

    UART_printf("Timer 1 Interrupt\n");

    /* Perform a context switch if necessary */
    //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}
