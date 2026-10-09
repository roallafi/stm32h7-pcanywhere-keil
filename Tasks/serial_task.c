#include "serial_task.h"
#include "uart_serial.h"
#include "protocol.h"
#include "config.h"

extern QueueHandle_t screen_queue;

void SerialTask(void *pvParameters) {
    Packet_t pkt;
    Packet_t recv;
    uint8_t screen_buffer[SCREEN_BUFFER_SIZE];

    while (1) {
        if (UART_DataAvailable()) {
            if (Protocol_RecvPacket(&recv) == 0) {
                if (recv.type == MSG_SCREEN) {
                    uint16_t offset = recv.seq * PACKET_SIZE;
                    memcpy(&screen_buffer[offset], recv.data, recv.length);
                    if (recv.seq >= 15) {
                        xQueueOverwrite(screen_queue, screen_buffer);
                    }
                }
                else if (recv.type == MSG_ACK) {
                    /* Connected */
                }
            }
        }

        vTaskDelay(10);
    }
}
