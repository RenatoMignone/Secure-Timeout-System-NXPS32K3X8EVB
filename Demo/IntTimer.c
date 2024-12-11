/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Demo includes. */
#include "IntTimer.h"
/* Library includes. */
#include "CMSIS/S32K3X8EVB.h"
#include "uart.h"

#define tmrTIMER_0_FREQUENCY	( 2UL )

/*
void vInitialiseTimers( void )
{
    CMSDK_TIMER0->INTCLEAR = ( 1ul <<  0 );
    CMSDK_TIMER0->RELOAD   = ( configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY );
    CMSDK_TIMER0->CTRL     = ( ( 1ul <<  3 ) | // Enable Timer interrupt.
                             ( 1ul <<  0 ) );  // Enable Timer.
    
    
    NVIC_SetPriority( TIMER0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER0_IRQn );
}

void TIMER0_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Clear the interrupt
    CMSDK_TIMER0->INTCLEAR = ( 1ul <<  0 );

    UART_printf("Timer 0 Interrupt\n");

    // Perform a context switch if necessary
    //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/

void vInitialiseTimers( void )
{
    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;  // Clear any pending interrupts
    S32K3X8_TIMER0->RELOAD = ( configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY );  // Set reload value
    S32K3X8_TIMER0->CTRL   = ( TIMER_CTRL_INTEN_Msk |  // Enable Timer interrupt
                               TIMER_CTRL_ENABLE_Msk );  // Enable Timer

    NVIC_SetPriority( TIMER0_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER0_IRQn );
}

void TIMER0_IRQHandler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;

    UART_printf("Timer 0 Interrupt\n");

    /* Perform a context switch if necessary */
    //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
