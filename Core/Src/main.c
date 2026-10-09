#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart_serial.h"
#include "protocol.h"
#include "serial_task.h"
#include "keyboard_task.h"
#include "display_task.h"
#include "usb_host_hid.h"

UART_HandleTypeDef huart1;
HCD_HandleTypeDef hhcd_usb_otg_hs;
QueueHandle_t screen_queue;
QueueHandle_t keyboard_queue;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USB_HOST_Init();
    MX_FSMC_Init();

    UART_Init(&huart1);
    USB_Keyboard_Init();

    screen_queue = xQueueCreate(2, SCREEN_BUFFER_SIZE);
    keyboard_queue = xQueueCreate(16, sizeof(uint8_t));

    if (screen_queue == NULL || keyboard_queue == NULL) {
        Error_Handler();
    }

    xTaskCreate(SerialTask, "SerialTask", TASK_STACK_SIZE * 4, NULL, SERIAL_TASK_PRIO, NULL);
    xTaskCreate(KeyboardTask, "KeyboardTask", TASK_STACK_SIZE * 4, NULL, KEYBOARD_TASK_PRIO, NULL);
    xTaskCreate(DisplayTask, "DisplayTask", TASK_STACK_SIZE * 4, NULL, DISPLAY_TASK_PRIO, NULL);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}

void SystemClock_Config(void) {
    /* Relevant STM32H7 clock configuration must be set in CubeMX or here. */
}

void MX_GPIO_Init(void) {
    /* GPIO setup for UART, LEDs, TFT control pins */
}

void MX_USART1_UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = UART_BAUD_RATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

void MX_USB_HOST_Init(void) {
    /* USB Host initialization goes here */
}

void MX_FSMC_Init(void) {
    /* TFT parallel interface setup goes here */
}

void Error_Handler(void) {
    while (1) {
        HAL_Delay(200);
    }
}
