/* Scheduler includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* Demo includes. */
#include "IntTimer.h"
/* Library includes. */
#include "sp804_timer.h"  // Include the appropriate header file for the SP804 timer

#define tmrTIMER_0_FREQUENCY	( 2UL )

/*******************************************************************/

// TO BE MOVED ?
#define PIT_TIMER_BASE_ADDR    0x40037000  // PIT base address

#define TIMER_SP804_CTRL_TIMEREN     (1 << 7)
#define TIMER_SP804_CTRL_TIMERMODE   (1 << 6)        // Bit 6:
#define TIMER_SP804_CTRL_INTENABLE   (1 << 5)
#define TIMER_SP804_CTRL_TIMERSIZE   (1 << 1)        // Bit 1: 0=16-bit, 1=32-bit
#define TIMER_SP804_CTRL_ONESHOT     (1 << 0)        // Bit 0: 0=wrapping, 1=one-shot

#define TIMER_SP804_CTRL_PRESCALE_1  (0 << 2)        // clk/1
#define TIMER_SP804_CTRL_PRESCALE_4  (1 << 2)        // clk/4
#define TIMER_SP804_CTRL_PRESCALE_8  (2 << 2)        // clk/8

// Define the IRQ number for PIT0
#define PIT0_IRQn 48

/*******************************************************************/

/*
void vInitialiseTimers( void )
{
	CMSDK_TIMER0->INTCLEAR = ( 1ul <<  0 );
	CMSDK_TIMER0->RELOAD   = ( configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY );
	CMSDK_TIMER0->CTRL     = ( ( 1ul <<  3 ) | // Enable Timer interrupt.
						     ( 1ul <<  0 ) );  // Enable Timer.
	NVIC_EnableIRQ( TIMER0_IRQn );
}
*/

/*
void vInitialiseTimers(void)
{
    // Clear the timer interrupt flag
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    
    // Set the timer period
    PIT->CHANNEL[0].LDVAL = (configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY) - 1;
    
    // Enable the timer and its interrupt
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
    
    // Enable the PIT module
    PIT->MCR &= ~PIT_MCR_MDIS_MASK;
    
    // Enable the interrupt in the NVIC
    NVIC_EnableIRQ(PIT0_IRQn);
}

void PIT0_IRQHandler(void)
{
    // Clear the interrupt flag
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    
    // Call the FreeRTOS handler
    xTimer0Handler();
}
*/

portBASE_TYPE xTimer0Handler(void)
{
    // Handle the timer interrupt
    // This function should be implemented according to your application needs
    // For example, you can use it to signal a task or increment a counter
    return pdTRUE;
}

void vInitialiseTimers(void)
{
    // Set the base address of the timer
    setTimerBaseAddress(PIT_TIMER_BASE_ADDR);

    // Initialize the timer with the desired load value, auto-reload, and interrupt enabled
    initTimer(configCPU_CLOCK_HZ / tmrTIMER_0_FREQUENCY, TIMER_SP804_CTRL_TIMERMODE, TIMER_SP804_CTRL_INTENABLE);

    // Start the timer
    startTimer();

    // Enable the interrupt in the NVIC
    //NVIC_EnableIRQ(PIT0_IRQn);
}

void PIT0_IRQHandler(void)
{
    // Clear the interrupt flag
    clearTimerIrq();

    // Call the FreeRTOS handler
    xTimer0Handler();
}
