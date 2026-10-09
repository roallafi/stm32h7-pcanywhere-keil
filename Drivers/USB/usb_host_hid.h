#ifndef USB_HOST_HID_H
#define USB_HOST_HID_H

#include <stdint.h>

void USB_Keyboard_Init(void);
int USB_Keyboard_GetKey(uint8_t *key);

#endif
