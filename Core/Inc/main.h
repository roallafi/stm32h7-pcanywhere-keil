#ifndef MAIN_H
#define MAIN_H

#include "stm32h7xx_hal.h"

extern UART_HandleTypeDef huart1;
extern HCD_HandleTypeDef hhcd_usb_otg_hs;

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_USB_HOST_Init(void);
void MX_FSMC_Init(void);
void Error_Handler(void);

#endif
