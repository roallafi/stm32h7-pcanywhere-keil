#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* System Configuration */
#define SYSTEM_CLOCK        400000000UL
#define HSE_VALUE           25000000UL

/* UART Configuration */
#define UART_BAUD_RATE     9600
#define UART_RX_BUFFER_SIZE 512
#define UART_TX_BUFFER_SIZE 512

/* Screen Configuration */
#define SCREEN_BUFFER_SIZE  4000
#define TEXT_COLS           80
#define TEXT_ROWS           25
#define DISPLAY_WIDTH       320
#define DISPLAY_HEIGHT      240
#define DISPLAY_BPP         16  /* RGB565 */

/* Packet Configuration */
#define PACKET_SIZE         256
#define PACKET_TIMEOUT      1000

/* Message Types */
#define MSG_HANDSHAKE       0x01
#define MSG_ACK             0x02
#define MSG_NAK             0x03
#define MSG_FILE_REQUEST    0x04
#define MSG_FILE_DATA       0x05
#define MSG_FILE_END        0x06
#define MSG_SCREEN          0x07
#define MSG_KEYBOARD        0x08
#define MSG_DISCONNECT      0x09
#define MSG_PING            0x0A

/* FreeRTOS Task Configuration */
#define TASK_STACK_SIZE     512
#define SERIAL_TASK_PRIO    3
#define KEYBOARD_TASK_PRIO  4
#define DISPLAY_TASK_PRIO   2
#define TIMER_TASK_PRIO     1

/* USB HID Configuration */
#define USB_HID_BUFFER_SIZE 64
#define HID_KEYBOARD_REPORT_SIZE 8

/* TFT Display Configuration */
#define TFT_FSMC_BANK       0
#define TFT_REFRESH_RATE    30  /* FPS */
#define TFT_CHAR_WIDTH      4   /* pixels */
#define TFT_CHAR_HEIGHT     10  /* pixels */

#endif /* CONFIG_H */
