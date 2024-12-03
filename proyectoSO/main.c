#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

// Definiciones de prioridades
#define HIGH_PRIORITY 2
#define LOW_PRIORITY 1

// Variables para medir latencia
unsigned long start_time, end_time, latency;
unsigned long latencies_arch1[5], latencies_arch2[5], latencies_arch3[5];
int arch1_count = 0, arch2_count = 0, arch3_count = 0;

// Variables para colas y semáforos
QueueHandle_t interruptQueue;
SemaphoreHandle_t semaphore;
TaskHandle_t interruptTaskHandle, controlTaskHandle;

// Variables para uso de recursos
unsigned long stack_high_watermark_arch1, stack_high_watermark_arch2, stack_high_watermark_arch3;
unsigned long heap_size_before, heap_size_after;
unsigned long total_execution_time_arch1, total_execution_time_arch2, total_execution_time_arch3;
unsigned long interrupt_count_arch1 = 0, interrupt_count_arch2 = 0, interrupt_count_arch3 = 0;
unsigned long cpu_usage_arch1 = 0, cpu_usage_arch2 = 0, cpu_usage_arch3 = 0;

// Funciones para simular tareas de interrupción y control
void interrupt_task_arch1(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch1_count < 5) {
        start_time = xTaskGetTickCount();  // Medir tiempo al iniciar la interrupción
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Simulación de interrupción
        end_time = xTaskGetTickCount();
        latency = (end_time - start_time) * portTICK_PERIOD_MS;
        latencies_arch1[arch1_count++] = latency;
        interrupt_count_arch1++;
        printf("Arquitectura 1 - Latencia: %ld ms\n", latency);
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Tiempo entre interrupciones
    }
    total_execution_time_arch1 = xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void control_task_arch1(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch1_count < 5) {
        printf("Control Task - Arquitectura 1\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Simulación de tarea de control
    }
    stack_high_watermark_arch1 = uxTaskGetStackHighWaterMark(NULL);
    total_execution_time_arch1 += xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void interrupt_task_arch2(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch2_count < 5) {
        int interrupt_msg = 1;
        xQueueSendToBack(interruptQueue, &interrupt_msg, 0);
        interrupt_count_arch2++;
        printf("Mensaje de interrupción enviado a la cola - Arquitectura 2\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    total_execution_time_arch2 = xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void control_task_arch2(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch2_count < 5) {
        int interrupt_msg;
        if (xQueueReceive(interruptQueue, &interrupt_msg, portMAX_DELAY) == pdPASS) {
            printf("Control Task - Arquitectura 2 procesando mensaje de interrupción\n");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Simulación de tarea de control
    }
    stack_high_watermark_arch2 = uxTaskGetStackHighWaterMark(NULL);
    total_execution_time_arch2 += xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void interrupt_task_arch3(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch3_count < 5) {
        if (xSemaphoreTake(semaphore, portMAX_DELAY) == pdTRUE) {
            start_time = xTaskGetTickCount();
            printf("Interrupción procesada - Arquitectura 3\n");
            end_time = xTaskGetTickCount();
            latency = (end_time - start_time) * portTICK_PERIOD_MS;
            latencies_arch3[arch3_count++] = latency;
            interrupt_count_arch3++;
            printf("Arquitectura 3 - Latencia: %ld ms\n", latency);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);  // Simulación de interrupción
    }
    total_execution_time_arch3 = xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void control_task_arch3(void *param) {
    unsigned long task_start_time = xTaskGetTickCount();
    while (arch3_count < 5) {
        printf("Control Task - Arquitectura 3\n");
        xSemaphoreGive(semaphore);  // Liberar semáforo para la tarea de interrupción
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Simulación de tarea de control
    }
    stack_high_watermark_arch3 = uxTaskGetStackHighWaterMark(NULL);
    total_execution_time_arch3 += xTaskGetTickCount() - task_start_time;
    vTaskDelete(NULL);
}

void app_main() {
    heap_size_before = xPortGetFreeHeapSize();  // Obtener el tamaño de heap antes de las tareas

    // Configuración de las arquitecturas

    // Arquitectura 1 - Prioridades estáticas
    printf("Ejecutando Arquitectura 1\n");
    xTaskCreatePinnedToCore(interrupt_task_arch1, "Interrupt Task Arch1", 2048, NULL, HIGH_PRIORITY, &interruptTaskHandle, 0);
    xTaskCreatePinnedToCore(control_task_arch1, "Control Task Arch1", 2048, NULL, LOW_PRIORITY, &controlTaskHandle, 0);

    // Esperar a que Arquitectura 1 termine
    vTaskDelay(6000 / portTICK_PERIOD_MS);  // Ajustar según el tiempo que tarde la Arquitectura 1

    // Arquitectura 2 - Comunicación con colas
    printf("Ejecutando Arquitectura 2\n");
    interruptQueue = xQueueCreate(10, sizeof(int));
    xTaskCreatePinnedToCore(interrupt_task_arch2, "Interrupt Task Arch2", 2048, NULL, HIGH_PRIORITY, NULL, 0);
    xTaskCreatePinnedToCore(control_task_arch2, "Control Task Arch2", 2048, NULL, LOW_PRIORITY, NULL, 0);

    // Esperar a que Arquitectura 2 termine
    vTaskDelay(6000 / portTICK_PERIOD_MS);  // Ajustar según el tiempo que tarde la Arquitectura 2

    // Arquitectura 3 - Sincronización con semáforos
    printf("Ejecutando Arquitectura 3\n");
    semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphore);  // Inicializar semáforo disponible
    xTaskCreatePinnedToCore(interrupt_task_arch3, "Interrupt Task Arch3", 2048, NULL, HIGH_PRIORITY, NULL, 0);
    xTaskCreatePinnedToCore(control_task_arch3, "Control Task Arch3", 2048, NULL, LOW_PRIORITY, NULL, 0);

    // Esperar a que Arquitectura 3 termine
    vTaskDelay(6000 / portTICK_PERIOD_MS);  // Ajustar según el tiempo que tarde la Arquitectura 3

    heap_size_after = xPortGetFreeHeapSize();  // Obtener el tamaño de heap después de las tareas

    // Calcular el promedio de latencias de cada arquitectura
    unsigned long total_arch1 = 0, total_arch2 = 0, total_arch3 = 0;
    for (int i = 0; i < 5; i++) {
        total_arch1 += latencies_arch1[i];
        total_arch2 += latencies_arch2[i];
        total_arch3 += latencies_arch3[i];
    }

    printf("\nPromedio de latencias:\n");
    printf("Arquitectura 1 - Promedio: %ld ms\n", total_arch1 / 5);
    printf("Arquitectura 2 - Promedio: %ld ms\n", total_arch2 / 5);
    printf("Arquitectura 3 - Promedio: %ld ms\n", total_arch3 / 5);

    // Mostrar métricas adicionales
    printf("\nMétricas adicionales:\n");
    printf("Arquitectura 1 - Uso de pila: %ld bytes\n", stack_high_watermark_arch1);
    printf("Arquitectura 2 - Uso de pila: %ld bytes\n", stack_high_watermark_arch2);
    printf("Arquitectura 3 - Uso de pila: %ld bytes\n", stack_high_watermark_arch3);

    printf("Arquitectura 1 - Tiempo total de ejecución: %ld ms\n", total_execution_time_arch1);
    printf("Arquitectura 2 - Tiempo total de ejecución: %ld ms\n", total_execution_time_arch2);
    printf("Arquitectura 3 - Tiempo total de ejecución: %ld ms\n", total_execution_time_arch3);

    printf("Arquitectura 1 - Número de interrupciones procesadas: %ld\n", interrupt_count_arch1);
    printf("Arquitectura 2 - Número de interrupciones procesadas: %ld\n", interrupt_count_arch2);
    printf("Arquitectura 3 - Número de interrupciones procesadas: %ld\n", interrupt_count_arch3);

    printf("Arquitectura 1 - Uso de CPU: %ld%%\n", cpu_usage_arch1);
    printf("Arquitectura 2 - Uso de CPU: %ld%%\n", cpu_usage_arch2);
    printf("Arquitectura 3 - Uso de CPU: %ld%%\n", cpu_usage_arch3);

    printf("\nTamaño de heap antes de las tareas: %ld bytes\n", heap_size_before);
    printf("Tamaño de heap después de las tareas: %ld bytes\n", heap_size_after);
}
