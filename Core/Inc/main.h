#ifndef MAIN_H
#define MAIN_H

#include "stm32h7xx_hal.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_bus.h"
#include <stdint.h>
#include <string.h>

/* Peripheral Handles */
extern UART_HandleTypeDef huart1;
extern HCD_HandleTypeDef hhcd_USB_OTG_HS;
extern FSMC_NORSRAM_TimingTypeDef Timing;
extern FSMC_NORSRAM_InitTypeDef Init;

/* FreeRTOS Queues */
extern QueueHandle_t screen_queue;
extern QueueHandle_t keyboard_queue;

/* LED Pins */
#define LED_GREEN_PORT   GPIOB
#define LED_GREEN_PIN    GPIO_PIN_0
#define LED_RED_PORT     GPIOB
#define LED_RED_PIN      GPIO_PIN_1
#define LED_BLUE_PORT    GPIOB
#define LED_BLUE_PIN     GPIO_PIN_14

/* TFT FSMC Pins */
#define TFT_CS_PORT      GPIOD
#define TFT_CS_PIN       GPIO_PIN_7
#define TFT_RD_PORT      GPIOD
#define TFT_RD_PIN       GPIO_PIN_4
#define TFT_WR_PORT      GPIOD
#define TFT_WR_PIN       GPIO_PIN_5
#define TFT_DC_PORT      GPIOD
#define TFT_DC_PIN       GPIO_PIN_11
#define TFT_RST_PORT     GPIOE
#define TFT_RST_PIN      GPIO_PIN_1

/* Function Prototypes */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_DMA_Init(void);
void MX_FSMC_Init(void);
void MX_USB_OTG_HS_HCD_Init(void);
void MX_FreeRTOS_Init(void);
void Error_Handler(void);

void LED_Init(void);
void LED_Green(uint8_t state);
void LED_Red(uint8_t state);
void LED_Blue(uint8_t state);

#endif /* MAIN_H */
