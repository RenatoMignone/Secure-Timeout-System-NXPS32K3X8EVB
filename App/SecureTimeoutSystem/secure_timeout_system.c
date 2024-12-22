/* FreeRTOS includes */
#include "FreeRTOS.h"
#include "task.h"
// #include "semphr.h" // TODO: uncomment (?)

/* Application includes */
#include "secure_timeout_system.h"
#include "globals.h"

/* Peripheral includes */
#include "uart.h"
#include "IntTimer.h"
#include "printf-stdarg.h"

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
// uint32_t userActivity = 0;                             // Overflow?
// uint32_t userActivityDetection = 0;                    // Overflow?
// uint32_t suspiciousActivity = 0;                       // Overflow?
// uint32_t suspiciousActivityDetection = 0;              // Overflow?
// static volatile int userActivity = 0;                  // Debug
// static volatile int userActivityDetection = 0;         // Debug
// static volatile int suspiciousActivity = 0;            // Debug
// static volatile int suspiciousActivityDetection = 0;   // Debug

/* Do these variables have to be implemented as sempahores ? */

/* Activity Detection counters */
static int userADCount = 0;
static int suspiciousADCount = 0;

/* Seed used to generate pseudo random numbers */
static uint32_t seed = 14536;

/* Custom function to generate a pseudo-random number */
uint32_t simpleRandom() 
{
    /* Linear Congruential Generator (LCG) parameters */
    seed = (seed * 1664525 + 1013904223); // Modulus is implicitly 2^32
    return seed;
}

void initSecureTimeoutSystem(void) 
{
    userActivity = 0;
    userActivityDetection = 0;
    suspiciousActivity = 0;
    suspiciousActivityDetection = 0;
}

/* Setters and Getters */
int getUserActivity(void) { return userActivity; }
int getSuspiciousActivity(void) { return suspiciousActivity; }
void setUserActivityDetection(int value) { userActivityDetection = value; }
void setSuspiciousActivityDetection(int value) { suspiciousActivityDetection = value; }

/*--------------------------------------------------------------------------------*/

void vStartSecureTimeoutSystem(void) 
{
    /* Initialise the secure timeout system */
    initSecureTimeoutSystem();

    /* Hardware initialisation */
    vInitialiseTimers();

    /* Create the tasks */
    xTaskCreate(vMonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
    xTaskCreate(vAlertTask,   "AlertTask",   configMINIMAL_STACK_SIZE, NULL, ALERT_TASK_PRIORITY,   NULL);
    xTaskCreate(vEventTask,   "EventTask",   configMINIMAL_STACK_SIZE, NULL, EVENT_TASK_PRIORITY,   NULL);  
}

static void vMonitorTask(void *pvParameters) 
{
    (void) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        if (userActivityDetection == 1) {
            printf("User activity detected!\n");
            userActivityDetection = 0;
            // Extra implementation
        } else {
            printf("No user activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vAlertTask(void *pvParameters) 
{
    (void) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(500));

    for (;;) {
        printf("Alert Task: suspiciousActivityDetection = %d\n", suspiciousActivityDetection); // Debug
        if (suspiciousActivityDetection == 1) {
            printf("!!!!!!!!!!!!!!!!!!! Suspicious activity detected !!!!!!!!!!!!!!!!!!!\n");
            suspiciousActivityDetection = 0;
            // Extra implementation
        } else {
            printf("No suspicious activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void vEventTask(void *pvParameters) 
{
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

        printf("Event Task: userActivity = %d\n", userActivity); // Debug
        printf("Event Task: suspiciousActivity = %d\n", suspiciousActivity); // Debug

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
