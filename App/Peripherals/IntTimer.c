/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

/* Application includes */
#include "globals.h"

/* Peripheral includes */
#include "uart.h"
#include "IntTimer.h"
#include "printf-stdarg.h"

/* Library includes. */
#include "S32K3X8EVB.h"

/* Timer 0 and Timer 1 frequencies */
#define tmrTIMER_0_FREQUENCY	( 2UL )
#define tmrTIMER_1_FREQUENCY	( 2UL )
#define tmrTIMER_2_FREQUENCY    ( 2UL )

/* Timer IRQ numbers */
#define TIMER0_IRQ_num 8
#define TIMER1_IRQ_num 9
#define TIMER2_IRQ_num 10

void vInitialiseTimers( my_bool verbose )
{
    if (verbose) printf("------------------- Initialization of Hardware Timers --------------------\n\n");

    /* Initialise Timer 0 */

    if (verbose) printf("Initialising Timer 0\n");

    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;             /* Clear any pending interrupts */
    S32K3X8_TIMER0->RELOAD   = ( configCPU_CLOCK_HZ /      /* Set reload value */
                                 tmrTIMER_0_FREQUENCY );
    S32K3X8_TIMER0->CTRL     = ( ( 1ul <<  3 ) |           /* Enable Timer interrupt. */
                                 ( 1ul <<  0 ) );          /* Enable Timer. */
    
    NVIC_SetPriority( TIMER0_IRQ_num, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER0_IRQ_num );

    if (verbose) printf("Timer 0 initialised\n");

    /* Initialise Timer 1 */

    if (verbose) printf("Initialising Timer 1\n");

    S32K3X8_TIMER1->INTCLR = TIMER_INTCLR_Msk;             /* Clear any pending interrupts */
    S32K3X8_TIMER1->RELOAD   = ( configCPU_CLOCK_HZ /      /* Set reload value */
                                 tmrTIMER_1_FREQUENCY );
    S32K3X8_TIMER1->CTRL     = ( ( 1ul <<  3 ) |           /* Enable Timer interrupt. */
                                 ( 1ul <<  0 ) );          /* Enable Timer. */

    NVIC_SetPriority( TIMER1_IRQ_num, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER1_IRQ_num );

    if (verbose) printf("Timer 1 initialised\n");

    /* Initialise Timer 2 */
    
    if (verbose) printf("Initialising Timer 2\n");

    S32K3X8_TIMER2->INTCLR = TIMER_INTCLR_Msk;             /* Clear any pending interrupts */
    S32K3X8_TIMER2->RELOAD = ( configCPU_CLOCK_HZ /        /* Set reload value */
                               tmrTIMER_2_FREQUENCY );
    S32K3X8_TIMER2->CTRL   = ( ( 1ul << 3 ) |              /* Enable Timer interrupt. */
                               ( 1ul << 0 ) );             /* Enable Timer. */
    NVIC_SetPriority( TIMER2_IRQ_num, configMAX_SYSCALL_INTERRUPT_PRIORITY );
    NVIC_EnableIRQ( TIMER2_IRQ_num );

    if (verbose) printf("Timer 2 initialised\n");

    if (verbose) printf("\n--------------------------------------------------------------------------\n");
    if (verbose) printf("\n");
}

void TIMER0_IRQHandler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    S32K3X8_TIMER0->INTCLR = TIMER_INTCLR_Msk;

    /* Main functionality */
    printf("Timer 0 Interrupt: looking for user activities...\n");
    userActivityDetection = (userActivity == 1) ? 1 : 0;

    /* Perform a context switch if necessary */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void TIMER1_IRQHandler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    S32K3X8_TIMER1->INTCLR = TIMER_INTCLR_Msk;

    /* Main functionality */
    printf("Timer 1 Interrupt: looking for suspicious activities...\n");
    suspiciousActivityDetection = (suspiciousActivity == 1) ? 1 : 0;

    /* Perform a context switch if necessary */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void TIMER2_IRQHandler( void )
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Clear the interrupt */
    S32K3X8_TIMER2->INTCLR = TIMER_INTCLR_Msk;

    /* Main functionality */
    /* Possible implementation */
    
    /* Perform a context switch if necessary */
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}