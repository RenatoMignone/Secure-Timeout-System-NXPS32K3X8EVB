/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"

/*Application includes */
#include "secure_timeout_system.h"
#include "globals.h"

/* Peripheral includes */
#include "uart.h"
#include "IntTimer.h"

/* Task priorities */
#define MONITOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define ALERT_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)
#define EVENT_TASK_PRIORITY   (tskIDLE_PRIORITY + 4)

/* Task functions */
static void vMonitorTask(void *pvParameters);
static void vAlertTask(void *pvParameters);
static void vEventTask(void *pvParameters);

/* Global variables */
int userActivity = 0;
int userActivityDetection = 0;
int suspiciousActivity = 0;
int suspiciousActivityDetection = 0;
static int userADCount = 0;
static int suspiciousADCount = 0;

/* Do these variables have to be implemented as sempahores ? */

/* Seed used to generate pseudo random numbers */
static uint32_t seed = 14536;

/* Custom function to generate a pseudo-random number */
uint32_t simpleRandom() 
{
    // Linear Congruential Generator (LCG) parameters
    seed = (seed * 1664525 + 1013904223); // Modulus is implicitly 2^32
    return seed;
}

void vStartSecureTimeoutSystem(void) 
{
    /* Hardware initialisation */
    vInitialiseTimers();

    /* Create the tasks */
    xTaskCreate(vMonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
    xTaskCreate(vAlertTask,   "AlertTask",   configMINIMAL_STACK_SIZE, NULL, ALERT_TASK_PRIORITY,   NULL);
    xTaskCreate(vEventTask,   "EventTask",   configMINIMAL_STACK_SIZE, NULL, EVENT_TASK_PRIORITY,   NULL);
    

}

static void vMonitorTask(void *pvParameters) {

    (void) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        if (userActivityDetection) {
            printf("User activity detected!\n");
            userActivityDetection = 0;
            // Extra implementation
        } else {
            printf("No user activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

static void vAlertTask(void *pvParameters) {

    (void) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        if (suspiciousActivityDetection) {
            printf("!!!!!!!!!!!!!!!!!!! Suspicious activity detected !!!!!!!!!!!!!!!!!!!\n");
            suspiciousActivityDetection = 0;
            // Extra implementation
        } else {
            printf("No suspicious activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

static void vEventTask(void *pvParameters) {

    (void) pvParameters;

    for (;;) {
        printf("\n@@@@@@@@@@@@@@@@@@@ Simulating events... @@@@@@@@@@@@@@@@@@@\n\n");
                  
        /* Reset Activities */
        userActivity = 0;
        suspiciousActivity = 0;

        if (simpleRandom() % 2 == 1) {
            userActivity = 1;
            userADCount++;
            printf("Simulated user activity.\n");
        } else {
            suspiciousActivity = 1;
            suspiciousADCount++;
            printf("Simulated suspicious activity.\n");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }

}
