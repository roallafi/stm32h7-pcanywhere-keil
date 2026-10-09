#include "tft_display.h"
#include "font_8x16.h"

static uint16_t dos_palette[16] = {
    0x0000, 0x001F, 0x07E0, 0x07FF,
    0xF800, 0xF81F, 0xFFE0, 0xFFFF,
    0x4210, 0x001F, 0x07E0, 0x07FF,
    0xF800, 0xF81F, 0xFFE0, 0xFFFF
};

void TFT_Init(void) {
    /* TFT controller initialization must be performed in CubeMX / hardware layer */
    TFT_Clear(0x0000);
}

void TFT_Clear(uint16_t color) {
    uint16_t x, y;
    for (y = 0; y < DISPLAY_HEIGHT; y++) {
        for (x = 0; x < DISPLAY_WIDTH; x++) {
            TFT_DrawPixel(x, y, color);
        }
    }
}

void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    /* Placeholder for actual TFT memory write */
    (void)x;
    (void)y;
    (void)color;
}

void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t ch, uint8_t attr) {
    uint8_t fg = attr & 0x0F;
    uint8_t bg = (attr >> 4) & 0x0F;
    uint16_t fg_color = dos_palette[fg];
    uint16_t bg_color = dos_palette[bg];
    const uint8_t *bitmap = font_8x16[(uint8_t)ch];

    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 8; col++) {
            uint8_t bit = (bitmap[row] >> (7 - col)) & 1;
            TFT_DrawPixel(x + col, y + row, bit ? fg_color : bg_color);
        }
    }
}

void TFT_RenderTextScreen(const uint8_t *screen_buffer) {
    uint16_t row, col;
    uint16_t x, y;

    for (row = 0; row < TEXT_ROWS; row++) {
        for (col = 0; col < TEXT_COLS; col++) {
            uint16_t idx = (row * TEXT_COLS + col) * 2;
            uint8_t ch = screen_buffer[idx];
            uint8_t attr = screen_buffer[idx + 1];
            x = col * 4;
            y = row * 10;
            TFT_DrawChar(x, y, ch, attr);
        }
    }
}
