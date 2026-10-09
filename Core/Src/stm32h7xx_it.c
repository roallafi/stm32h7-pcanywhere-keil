#include "main.h"
#include "config.h"

/* Global UART RX Buffer */
uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
static uint16_t uart_rx_index = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        uart_rx_index = (uart_rx_index + 1) % UART_RX_BUFFER_SIZE;
        HAL_UART_Receive_IT(huart, &uart_rx_buffer[uart_rx_index], 1);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* Timer callback for periodic tasks if needed */
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    /* GPIO External Interrupt Callback */
}

void OTG_HS_IRQHandler(void) {
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&huart1);
}

void NMI_Handler(void) {
}

void HardFault_Handler(void) {
    while (1) {
        LED_Red(1);
    }
}

void MemManage_Handler(void) {
    while (1) {
        LED_Red(1);
    }
}

void BusFault_Handler(void) {
    while (1) {
        LED_Red(1);
    }
}

void UsageFault_Handler(void) {
    while (1) {
        LED_Red(1);
    }
}

void SVC_Handler(void) {
}

void DebugMon_Handler(void) {
}

void PendSV_Handler(void) {
}

void SysTick_Handler(void) {
    HAL_IncTick();
}
