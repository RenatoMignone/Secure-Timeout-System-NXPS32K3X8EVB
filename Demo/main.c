#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "queue.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


#define mainTASK_PRIORITY (tskIDLE_PRIORITY +2)

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void TaskA(void *pvParameters);
static TimerHandle_t one_shot_timer = NULL;
static TimerHandle_t auto_reload_timer = NULL;


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//we need to define a callback function to handle the timers
//only one handling function to use in both timers
void myTimerCallback(TimerHandle_t xTimer){

  //we pass the parameter of the hanTimerHandle_tdler of a given timer
  if( (uint32_t) pvTimerGetTimerID(xTimer) == 0){
    UART_printf("Timer 0 Expired\n");
  }
  if( (uint32_t) pvTimerGetTimerID(xTimer) == 1){
    UART_printf("Timer 1 Expired\n");
  }
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

  (void) argc;
  (void) argv;

  UART_init();

  xTaskCreate(
    TaskA,
    "TaskA",
    configMINIMAL_STACK_SIZE,
    NULL,
    mainTASK_PRIORITY,
    NULL
  );

  UART_printf("Ready to run the scheduler..\n");
  vTaskStartScheduler();

  for( ; ; );
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void TaskA(void *pvParameters){
  
  (void) pvParameters;
  
  one_shot_timer = xTimerCreate(
    //name of the timer
    "One-shot-timer",
    //timer period in ticks
    pdMS_TO_TICKS(2000),
    //oneshot or not
    pdFALSE,
    (void *) 0,
    myTimerCallback
  );

    auto_reload_timer = xTimerCreate(
    //name of the timer
    "Auto-Reload-timer",
    //timer period in ticks
    pdMS_TO_TICKS(1000),
    //oneshot or auto reload
    pdTRUE,
    //id del timer
    (void *) 1,
    myTimerCallback
  );

  if(one_shot_timer == NULL || auto_reload_timer == NULL){
    UART_printf("ERROR: impossible to create timer \n");
  }
  
  UART_printf("Starting Timers \n");
  
  //stat the timer, the second param is the maximum delay to wait in case the command queue is full. This parameter in particula basically means wait forever
  xTimerStart(one_shot_timer,portMAX_DELAY);
  xTimerStart(auto_reload_timer, portMAX_DELAY);

  vTaskDelete(NULL);
  //we will have the timers running but not the task.

}

