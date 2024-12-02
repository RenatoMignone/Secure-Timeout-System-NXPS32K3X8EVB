#include "FreeRTOS.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <task.h>
#include <uart.h>
#define mainTASK_PRIORITY (tskIDLE_PRIORITY + 2)



void task1(void *pvParameters);


int main(int argc, char **argv){

	(void) argc;
	(void) argv;

    	UART_init();
  xTaskCreate(
      task1,
      "Task1",
      configMINIMAL_STACK_SIZE,
      NULL,
      mainTASK_PRIORITY,
      NULL
      );
  

	// Give control to the scheduler
	vTaskStartScheduler();

	// If everything ok should never reach here
    for( ; ; );
} 


void task1(void *pvParameters){
  (void)pvParameters;

  for(;;){
	
	UART_printf("Hello world!\n");	
	
    }
  }





