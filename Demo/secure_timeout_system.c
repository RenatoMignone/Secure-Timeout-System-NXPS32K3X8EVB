#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "IntTimer.h"
#include "secure_timeout_system.h"

#define MONITOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define ALERT_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)
#define EVENT_TASK_PRIORITY   (tskIDLE_PRIORITY + 4)

static void vMonitorTask(void *pvParameters);
static void vAlertTask(void *pvParameters);
static void vEventTask(void *pvParameters);

static int userActivityDetected = 0;
static int suspiciousActivityDetected = 0;

void vStartSecureTimeoutSystem(void) {
    xTaskCreate(vMonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
    xTaskCreate(vAlertTask, "AlertTask", configMINIMAL_STACK_SIZE, NULL, ALERT_TASK_PRIORITY, NULL);
    xTaskCreate(vEventTask, "EventTask", configMINIMAL_STACK_SIZE, NULL, EVENT_TASK_PRIORITY, NULL);
}

static void vMonitorTask(void *pvParameters) {

    (void) pvParameters;

    // Initialize hardware timers
    vInitialiseTimers();

    for (;;) {
        if (userActivityDetected) {
            UART_printf("User activity detected. Resetting timer...\n");
            userActivityDetected = 0;
            // Reset hardware timer here
        } else {
            UART_printf("No user activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

}

static void vAlertTask(void *pvParameters) {

    (void) pvParameters;

    for (;;) {
        if (suspiciousActivityDetected) {
            UART_printf("Suspicious activity detected! Logging alert...\n");
            suspiciousActivityDetected = 0;
            // Log alert here
        } else {
            UART_printf("No suspicious activity detected.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}

static void vEventTask(void *pvParameters) {

    (void) pvParameters;

    for (;;) {
        // Simulate events
        UART_printf("Simulating events...\n");
        // Randomly simulate user activity or suspicious activity
        if (rand() % 2 == 0) {
            userActivityDetected = 1;
            UART_printf("Simulated user activity.\n");
        } else {
            suspiciousActivityDetected = 1;
            UART_printf("Simulated suspicious activity.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

}
