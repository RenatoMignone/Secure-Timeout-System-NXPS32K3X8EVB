#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "queue.h"
#include "IntTimer.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define mainTASK_PRIORITY (tskIDLE_PRIORITY +2)

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void TaskA(void *pvParameters);
void TaskB(void *pvParameters);

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

  (void) argc;
  (void) argv;

  UART_init();

  // Initialize the hardware timers
  vInitialiseTimers();

  xTaskCreate(
    TaskA,
    "TaskA",
    configMINIMAL_STACK_SIZE,
    NULL,
    mainTASK_PRIORITY +1,
    NULL
  );

  xTaskCreate(TaskB, "TaskB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY +1, NULL);

  UART_printf("Ready to run the scheduler..\n");
  vTaskStartScheduler();

  for( ; ; );

}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void TaskA(void *pvParameters){

    (void) pvParameters;

    for( ;; ){
        // TaskA can perform other operations here
        UART_printf("\palle\n");
        // vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }

}

void TaskB(void *pvParameters){
  (void ) pvParameters;

  for(;;){
      UART_printf("\nculo\n");
  }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* This function will get called if a task overflows its stack. */
    (void)xTask;
    (void)pcTaskName;
    UART_printf("Stack overflow in task: ");
    UART_printf(pcTaskName);
    UART_printf("\n");
    taskDISABLE_INTERRUPTS();
    for( ;; );
}