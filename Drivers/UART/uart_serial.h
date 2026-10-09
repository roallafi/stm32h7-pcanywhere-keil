#ifndef UART_SERIAL_H
#define UART_SERIAL_H

#include <stdint.h>
#include "stm32h7xx_hal.h"

#define UART_RX_BUFFER_SIZE 512
#define UART_TX_BUFFER_SIZE 512

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
    uint16_t rx_head;
    uint16_t rx_tail;
} UART_Dev_t;

void UART_Init(UART_HandleTypeDef *huart);
void UART_SendByte(uint8_t byte);
void UART_SendData(const uint8_t *data, uint16_t len);
int UART_RecvByte(uint8_t *byte);
int UART_RecvData(uint8_t *buffer, uint16_t max_len);
int UART_DataAvailable(void);

#endif
