#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart_serial.h"
#include "protocol.h"
#include "tft_display.h"
#include "usb_host_hid.h"
#include "serial_task.h"
#include "keyboard_task.h"
#include "display_task.h"

/* Peripheral Handles */
UART_HandleTypeDef huart1;
HCD_HandleTypeDef hhcd_USB_OTG_HS;

/* FreeRTOS Queues */
QueueHandle_t screen_queue = NULL;
QueueHandle_t keyboard_queue = NULL;

int main(void) {
    /* HAL Initialization */
    HAL_Init();
    
    /* System Clock Configuration */
    SystemClock_Config();
    
    /* GPIO Initialization */
    MX_GPIO_Init();
    
    /* UART Initialization */
    MX_USART1_UART_Init();
    
    /* DMA Initialization */
    MX_DMA_Init();
    
    /* FSMC Initialization (TFT) */
    MX_FSMC_Init();
    
    /* USB Host Initialization */
    MX_USB_OTG_HS_HCD_Init();
    
    /* Driver Initialization */
    LED_Init();
    UART_Init(&huart1);
    TFT_Init();
    USB_Keyboard_Init();
    
    /* Create FreeRTOS Queues */
    screen_queue = xQueueCreate(2, SCREEN_BUFFER_SIZE);
    keyboard_queue = xQueueCreate(16, sizeof(uint8_t));
    
    if (screen_queue == NULL || keyboard_queue == NULL) {
        LED_Red(1);
        Error_Handler();
    }
    
    /* Create FreeRTOS Tasks */
    if (xTaskCreate(SerialTask, "SerialTask", configMINIMAL_STACK_SIZE * 4,
                   NULL, SERIAL_TASK_PRIO, NULL) != pdPASS) {
        LED_Red(1);
        Error_Handler();
    }
    
    if (xTaskCreate(KeyboardTask, "KeyboardTask", configMINIMAL_STACK_SIZE * 4,
                   NULL, KEYBOARD_TASK_PRIO, NULL) != pdPASS) {
        LED_Red(1);
        Error_Handler();
    }
    
    if (xTaskCreate(DisplayTask, "DisplayTask", configMINIMAL_STACK_SIZE * 4,
                   NULL, DISPLAY_TASK_PRIO, NULL) != pdPASS) {
        LED_Red(1);
        Error_Handler();
    }
    
    /* LED Green - System Ready */
    LED_Green(1);
    
    /* Start FreeRTOS Scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here */
    while (1);
    
    return 0;
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    
    __HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 160;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV4;
    
    if (HAL_RCC_ClkConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1 |
                                               RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_D2PCLK2;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
    
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Error_Handler();
    }
}

void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    
    /* LED GPIO Configuration */
    GPIO_InitStruct.Pin = LED_GREEN_PIN | LED_RED_PIN | LED_BLUE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /* UART1 GPIO Configuration PA9 -> TX, PA10 -> RX */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    /* TFT Control Pins */
    GPIO_InitStruct.Pin = TFT_RD_PIN | TFT_WR_PIN | TFT_DC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = TFT_RST_PIN;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
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
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
    
    HAL_UART_Receive_IT(&huart1, (uint8_t *)uart_rx_buffer, 1);
}

void MX_DMA_Init(void) {
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
}

void MX_FSMC_Init(void) {
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    FSMC_NORSRAM_InitTypeDef Init = {0};
    
    __HAL_RCC_FMC_CLK_ENABLE();
    
    /* FSMC GPIO Configuration */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                          GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                          GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 |
                          GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    /* FSMC Configuration */
    Init.NSBank = FSMC_NORSRAM_BANK1;
    Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    Init.WaitSignalActive = FSMC_WAIT_TIMING_DURING_ALL_CYCLES;
    Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    Init.ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC;
    
    Timing.AddressSetupTime = 15;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 60;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 0;
    Timing.DataLatency = 0;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    
    HAL_FSMC_NORSRAM_Init(FSMC_NORSRAM_DEVICE, &Init);
    HAL_FSMC_NORSRAM_Timing_Init(FSMC_NORSRAM_DEVICE, &Timing, FSMC_NORSRAM_BANK1);
}

void MX_USB_OTG_HS_HCD_Init(void) {
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __HAL_RCC_USB_OTG_HS_ULPI_CLK_ENABLE();
    
    hhcd_USB_OTG_HS.Instance = USB_OTG_HS;
    hhcd_USB_OTG_HS.Init.Host_channels = 16;
    hhcd_USB_OTG_HS.Init.speed = HCD_SPEED_HIGH;
    hhcd_USB_OTG_HS.Init.dma_enable = ENABLE;
    hhcd_USB_OTG_HS.Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd_USB_OTG_HS.Init.Sof_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.low_power_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.vbus_sensing_enable = DISABLE;
    hhcd_USB_OTG_HS.Init.use_dedicated_ep1 = DISABLE;
    hhcd_USB_OTG_HS.Init.use_external_vbus = DISABLE;
    
    if (HAL_HCD_Init(&hhcd_USB_OTG_HS) != HAL_OK) {
        Error_Handler();
    }
    
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}

void LED_Init(void) {
    LED_Green(0);
    LED_Red(0);
    LED_Blue(0);
}

void LED_Green(uint8_t state) {
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_Red(uint8_t state) {
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LED_Blue(uint8_t state) {
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void Error_Handler(void) {
    LED_Red(1);
    while (1) {
        HAL_Delay(500);
    }
}

void assert_failed(uint8_t *file, uint32_t line) {
    LED_Red(1);
    while (1);
}
