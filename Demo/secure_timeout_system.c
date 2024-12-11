#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "uart.h"
#include "IntTimer.h"
#include "secure_timeout_system.h"

#define MONITOR_TASK_PRIORITY (tskIDLE_PRIORITY + 2)
#define ALERT_TASK_PRIORITY   (tskIDLE_PRIORITY + 3)

static TimerHandle_t activity_timer = NULL;
static TimerHandle_t suspicious_timer = NULL;

static void vMonitorTask(void *pvParameters);
static void vAlertTask(void *pvParameters);
static void vActivityTimerCallback(TimerHandle_t xTimer);
static void vSuspiciousTimerCallback(TimerHandle_t xTimer);

void vStartSecureTimeoutSystem(void) {
    xTaskCreate(vMonitorTask, "MonitorTask", configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
    xTaskCreate(vAlertTask, "AlertTask", configMINIMAL_STACK_SIZE, NULL, ALERT_TASK_PRIORITY, NULL);
}

static void vMonitorTask(void *pvParameters) {
    (void) pvParameters;

    activity_timer = xTimerCreate("ActivityTimer", pdMS_TO_TICKS(5000), pdFALSE, (void *) 0, vActivityTimerCallback);
    suspicious_timer = xTimerCreate("SuspiciousTimer", pdMS_TO_TICKS(10000), pdTRUE, (void *) 1, vSuspiciousTimerCallback);

    if (activity_timer == NULL || suspicious_timer == NULL) {
        UART_printf("ERROR: Failed to create timers\n");
        vTaskDelete(NULL);
    }

    xTimerStart(activity_timer, portMAX_DELAY);
    xTimerStart(suspicious_timer, portMAX_DELAY);

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

static void vActivityTimerCallback(TimerHandle_t xTimer) {
    UART_printf("Activity timeout detected!\n");
    // Log the event and take necessary actions
}

static void vSuspiciousTimerCallback(TimerHandle_t xTimer) {
    UART_printf("Suspicious activity detected!\n");
    // Log the event and take necessary actions
}
