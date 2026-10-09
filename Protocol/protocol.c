#include "protocol.h"
#include "uart_serial.h"

void Protocol_CreatePacket(Packet_t *pkt, uint8_t type, uint8_t seq, const uint8_t *data, uint16_t len) {
    pkt->type = type;
    pkt->seq = seq;
    pkt->length = len;

    if (data && len > 0) {
        memcpy(pkt->data, data, len < PACKET_SIZE ? len : PACKET_SIZE);
    }

    pkt->checksum = Protocol_CalculateChecksum(pkt);
}

uint8_t Protocol_CalculateChecksum(const Packet_t *pkt) {
    uint8_t sum = 0;
    int i;

    sum += pkt->type;
    sum += pkt->seq;
    sum += (uint8_t)(pkt->length & 0xFF);
    sum += (uint8_t)(pkt->length >> 8);

    for (i = 0; i < pkt->length && i < PACKET_SIZE; i++) {
        sum += pkt->data[i];
    }

    return (uint8_t)(~sum + 1);
}

int Protocol_VerifyPacket(const Packet_t *pkt) {
    return pkt->checksum == Protocol_CalculateChecksum(pkt) ? 0 : -1;
}

int Protocol_SendPacket(const Packet_t *pkt) {
    uint8_t *buffer = (uint8_t *)pkt;
    int i;

    UART_SendByte(0xAA);
    UART_SendByte(0x55);

    for (i = 0; i < (int)sizeof(Packet_t); i++) {
        UART_SendByte(buffer[i]);
    }

    return 0;
}

int Protocol_RecvPacket(Packet_t *pkt) {
    uint8_t sync1, sync2;
    uint8_t *buffer = (uint8_t *)pkt;
    int i;
    int timeout;

    timeout = 50000;
    while (timeout--) {
        if (UART_RecvByte(&sync1) == 0) {
            if (sync1 == 0xAA) {
                if (UART_RecvByte(&sync2) == 0 && sync2 == 0x55) {
                    break;
                }
            }
        }
    }

    if (timeout <= 0) {
        return -1;
    }

    for (i = 0; i < (int)sizeof(Packet_t); i++) {
        timeout = 10000;
        while (timeout-- && UART_RecvByte(&buffer[i]) < 0) {
        }
        if (timeout <= 0) {
            return -1;
        }
    }

    return Protocol_VerifyPacket(pkt);
}
