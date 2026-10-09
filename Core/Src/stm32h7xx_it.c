#include "main.h"

void __attribute__((weak)) __aeabi_assert(void) {}

void HAL_MspInit(void) { }
void HAL_UART_MspInit(UART_HandleTypeDef *huart) { (void)huart; }
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) { (void)huart; }

void SysTick_Handler(void) {
    HAL_IncTick();
    osSystickHandler();
}
