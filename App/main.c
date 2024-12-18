/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* Standard includes */
// #include <stdio.h>
// #include <string.h>
// #include <limits.h>

/* Application includes */
#include "secure_timeout_system.h"

/* Peripheral includes */
#include "uart.h"
#include "IntTimer.h"
#include "printf-stdarg.h"

/* Task priorities */
#define mainTASK_PRIORITY (tskIDLE_PRIORITY + 2)

/* Extra functions */
extern void vStartSecureTimeoutSystem(void);

/*--------------------------------------------------------------------------------*/

int main(int argc, char **argv) {

    (void) argc;
    (void) argv;

    /* Hardware initialisation. */
    UART_init();

    printf("=========================== Starting the Main ============================\n\n");

    /* Start the secure timeout system */
    vStartSecureTimeoutSystem();

    printf("Ready to run the scheduler...\n");
    vTaskStartScheduler();

    for (;;);

}

/*--------------------------------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {

    /* This function will get called if a task overflows its stack. */

    (void)xTask;
    (void)pcTaskName;

    printf("Stack overflow in task: ");
    printf(pcTaskName);
    printf("\n");
    taskDISABLE_INTERRUPTS();

    for (;;);

}
