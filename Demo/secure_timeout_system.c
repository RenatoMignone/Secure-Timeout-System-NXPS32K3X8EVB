#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "IntTimer.h"
#include "secure_timeout_system.h"

#define MONITOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define ALERT_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)

static TimerHandle_t activity_timer = NULL;
static TimerHandle_t suspicious_timer = NULL;

static void vMonitorTask(void *pvParameters);
static void vAlertTask(void *pvParameters);

void vStartSecureTimeoutSystem(void) {
    xTaskCreate(vMonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
    xTaskCreate(vAlertTask, "AlertTask", configMINIMAL_STACK_SIZE, NULL, ALERT_TASK_PRIORITY, NULL);
}

static void vMonitorTask(void *pvParameters) {

    (void) pvParameters;

    // Initialize hardware timers
    vInitialiseTimers();

    for (;;) {
        // Simulate user activity
        UART_printf("Simulating user activity...\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

}

static void vAlertTask(void *pvParameters) {

    (void) pvParameters;

    for (;;) {
        // Wait for alerts and handle them
        UART_printf("Alert Task Running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
