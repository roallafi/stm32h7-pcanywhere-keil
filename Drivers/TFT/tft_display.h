#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <stdint.h>

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define TEXT_COLS      80
#define TEXT_ROWS      25
#define SCREEN_BUFFER_SIZE 4000

void TFT_Init(void);
void TFT_Clear(uint16_t color);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t ch, uint8_t attr);
void TFT_RenderTextScreen(const uint8_t *screen_buffer);

#endif
