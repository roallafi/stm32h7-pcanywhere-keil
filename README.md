# STM32H7 DOS Remote Desktop Client

STM32H7 FreeRTOS project for remote DOS desktop control via serial and USB keyboard.

## Hardware
- MCU: STM32H7 (STM32H743 recommended)
- RTOS: FreeRTOS
- Display: TFT LCD 320x240, RGB565, 40-pin parallel
- Input: USB HID keyboard
- Communication: UART RS-232 / COM port to DOS PC

## Functional goal
This firmware acts as a bridge between a DOS machine and a local TFT display + USB keyboard:
- receives the DOS 80x25 text screen (4KB = ASCII + attribute)
- renders it on the TFT display
- reads USB HID keyboard input
- forwards keystrokes to the DOS PC over serial

## Protocol
The serial protocol is kept compatible with the DOS-side implementation:
- MSG_HANDSHAKE = 0x01
- MSG_ACK = 0x02
- MSG_SCREEN = 0x07
- MSG_KEYBOARD = 0x08
- MSG_DISCONNECT = 0x09
- MSG_PING = 0x0A

The DOS screen buffer is transmitted in 80x25 text mode:
- 2000 character cells
- each cell = 2 bytes: ASCII + attribute
- total = 4000 bytes

## Rendering model
- 80 columns x 25 rows text buffer
- LCD width = 320, LCD height = 240
- Each character cell approximates 4x10 pixels for full coverage
- Font is a simple 8x16 or 8x12 bitmap
- Foreground/background colors come from the DOS attribute byte

## Structure
```
stm32h7-pcanywhere-keil/
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── config.h
│   │   └── stm32h7xx_it.h
│   └── Src/
│       ├── main.c
│       ├── stm32h7xx_it.c
│       └── system_stm32h7xx.c
├── Drivers/
│   ├── UART/
│   │   ├── uart_serial.h
│   │   └── uart_serial.c
│   ├── TFT/
│   │   ├── tft_init.h
│   │   ├── tft_init.c
│   │   ├── tft_display.h
│   │   └── tft_display.c
│   ├── USB/
│   │   ├── usb_host_hid.h
│   │   └── usb_host_hid.c
│   └── Font/
│       ├── font_8x16.h
│       └── font_8x16.c
├── Protocol/
│   ├── protocol.h
│   └── protocol.c
├── Tasks/
│   ├── serial_task.h
│   ├── serial_task.c
│   ├── keyboard_task.h
│   ├── keyboard_task.c
│   ├── display_task.h
│   └── display_task.c
├── README.md
├── STM32H7_PCAnywhere.vcxproj
└── FileList.txt
```

## Notes
This project is a practical starting point for development in Keil MDK. It is not marked as final production firmware and is intended for use in a controlled embedded development flow.
