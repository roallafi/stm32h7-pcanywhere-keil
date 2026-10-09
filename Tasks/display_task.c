#include "display_task.h"
#include "tft_display.h"
#include "config.h"

extern QueueHandle_t screen_queue;

void DisplayTask(void *pvParameters) {
    uint8_t screen_buffer[SCREEN_BUFFER_SIZE];
    BaseType_t ret;

    TFT_Init();
    TFT_Clear(0x0000);

    while (1) {
        ret = xQueueReceive(screen_queue, screen_buffer, portMAX_DELAY);
        if (ret == pdTRUE) {
            TFT_RenderTextScreen(screen_buffer);
        }
        vTaskDelay(20);
    }
}
