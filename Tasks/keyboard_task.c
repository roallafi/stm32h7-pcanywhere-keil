#include "keyboard_task.h"
#include "usb_host_hid.h"
#include "protocol.h"
#include "uart_serial.h"

extern QueueHandle_t keyboard_queue;

void KeyboardTask(void *pvParameters) {
    Packet_t pkt;
    uint8_t key;

    while (1) {
        if (USB_Keyboard_GetKey(&key) == 0) {
            Protocol_CreatePacket(&pkt, MSG_KEYBOARD, 0, &key, 1);
            Protocol_SendPacket(&pkt);
        }

        vTaskDelay(10);
    }
}
