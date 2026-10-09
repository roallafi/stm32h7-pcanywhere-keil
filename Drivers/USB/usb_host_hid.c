#include "usb_host_hid.h"
#include "FreeRTOS.h"
#include "queue.h"

extern QueueHandle_t keyboard_queue;

static uint8_t hid_queue[16];
static uint8_t head = 0;
static uint8_t tail = 0;

void USB_Keyboard_Init(void) {
    head = 0;
    tail = 0;
}

int USB_Keyboard_GetKey(uint8_t *key) {
    if (head == tail) {
        return -1;
    }

    *key = hid_queue[tail];
    tail = (tail + 1) % 16;
    return 0;
}

void USB_Keyboard_Enqueue(uint8_t key) {
    uint8_t next = (head + 1) % 16;
    if (next != tail) {
        hid_queue[head] = key;
        head = next;
    }
}
