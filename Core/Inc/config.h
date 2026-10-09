#ifndef CONFIG_H
#define CONFIG_H

#define UART_BAUD_RATE      9600
#define UART_RX_BUFFER_SIZE 512
#define UART_TX_BUFFER_SIZE 512
#define PACKET_SIZE         256

#define SCREEN_BUFFER_SIZE  4000
#define TEXT_COLS           80
#define TEXT_ROWS           25
#define DISPLAY_WIDTH       320
#define DISPLAY_HEIGHT      240
#define CHAR_WIDTH          8
#define CHAR_HEIGHT         16

#define MSG_HANDSHAKE       0x01
#define MSG_ACK             0x02
#define MSG_NAK             0x03
#define MSG_SCREEN          0x07
#define MSG_KEYBOARD        0x08
#define MSG_DISCONNECT      0x09
#define MSG_PING            0x0A

#define TASK_STACK_SIZE     512
#define SERIAL_TASK_PRIO    3
#define KEYBOARD_TASK_PRIO  4
#define DISPLAY_TASK_PRIO   2

#endif
