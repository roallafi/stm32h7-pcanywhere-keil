#include "uart_serial.h"

static UART_Dev_t uart_dev;

void UART_Init(UART_HandleTypeDef *huart) {
    uart_dev.huart = huart;
    uart_dev.rx_head = 0;
    uart_dev.rx_tail = 0;

    HAL_UART_Receive_IT(huart, &uart_dev.rx_buffer[0], 1);
}

void UART_SendByte(uint8_t byte) {
    HAL_UART_Transmit(uart_dev.huart, &byte, 1, 1000);
}

void UART_SendData(const uint8_t *data, uint16_t len) {
    HAL_UART_Transmit(uart_dev.huart, (uint8_t *)data, len, 1000);
}

int UART_RecvByte(uint8_t *byte) {
    if (uart_dev.rx_head == uart_dev.rx_tail) {
        return -1;
    }

    *byte = uart_dev.rx_buffer[uart_dev.rx_tail];
    uart_dev.rx_tail = (uart_dev.rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return 0;
}

int UART_RecvData(uint8_t *buffer, uint16_t max_len) {
    uint16_t i = 0;
    while (i < max_len) {
        if (UART_RecvByte(&buffer[i]) != 0) {
            break;
        }
        i++;
    }
    return i;
}

int UART_DataAvailable(void) {
    return uart_dev.rx_head != uart_dev.rx_tail;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart == uart_dev.huart) {
        uint16_t next = (uart_dev.rx_head + 1) % UART_RX_BUFFER_SIZE;
        if (next != uart_dev.rx_tail) {
            uart_dev.rx_head = next;
        }
        HAL_UART_Receive_IT(huart, &uart_dev.rx_buffer[uart_dev.rx_head], 1);
    }
}
