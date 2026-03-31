/*
 * ssd1306.c
 *
 *  Created on: Mar 16, 2026
 *      Author: Salih
 */

#include "ssd1306.h"
#include <string.h>
#include "ssd1306_fonts.h"


#define SSD1306_INCLUDE_FONT_7x10


uint8_t SSD1306_Buffer[1024];

static SSD1306_t SSD1306;


void ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t data) {
    // HAL_I2C_Mem_Write sends the Register/Control byte (0x00) for you
    HAL_I2C_Mem_Write(hi2c, SSD1306_I2C_ADDR << 1, 0x00, 1, &data, 1, 100);
}

void ssd1306_UpdateScreen(I2C_HandleTypeDef *hi2c) {
    // 0x40 is the "Data" control byte for SSD1306
    // We use Mem_Write_DMA to send the whole 1024 byte buffer
	if (hi2c->State == HAL_I2C_STATE_READY) {
		HAL_I2C_Mem_Write_DMA(hi2c, SSD1306_I2C_ADDR << 1, 0x40, 1, SSD1306_Buffer, 1024);
	}
}

void ssd1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
    if(x >= 128 || y >= 64) return;

    if(color) {
        // Find the byte in the buffer and set the specific bit
        // y >> 3 is the same as y / 8 (finding the page)
        // y & 0x07 is the same as y % 8 (finding the bit in that page)
        SSD1306_Buffer[x + (y >> 3) * 128] |= (1 << (y & 0x07));
    } else {
        SSD1306_Buffer[x + (y >> 3) * 128] &= ~(1 << (y & 0x07));
    }
}

void ssd1306_Init(I2C_HandleTypeDef *hi2c) {
    // 1. Wait for the screen to boot up
    HAL_Delay(100);

    // 2. Initialization Sequence
    ssd1306_WriteCommand(hi2c, 0xAE); // Display OFF

    ssd1306_WriteCommand(hi2c, 0x20); // Set Memory Addressing Mode
    ssd1306_WriteCommand(hi2c, 0x00); // 00b = Horizontal Addressing Mode (IMPORTANT!)

    ssd1306_WriteCommand(hi2c, 0xB0); // Set Page Start Address for Page Addressing Mode

    ssd1306_WriteCommand(hi2c, 0xC8); // Set COM Output Scan Direction

    ssd1306_WriteCommand(hi2c, 0x00); // Set low column address
    ssd1306_WriteCommand(hi2c, 0x10); // Set high column address

    ssd1306_WriteCommand(hi2c, 0x40); // Set start line address

    ssd1306_WriteCommand(hi2c, 0x81); // Set contrast control
    ssd1306_WriteCommand(hi2c, 0xFF); // Max brightness

    ssd1306_WriteCommand(hi2c, 0xA1); // Set segment re-map 0 to 127
    ssd1306_WriteCommand(hi2c, 0xA6); // Set normal display (not inverse)

    ssd1306_WriteCommand(hi2c, 0xA8); // Set multiplex ratio(1 to 64)
    ssd1306_WriteCommand(hi2c, 0x3F);

    ssd1306_WriteCommand(hi2c, 0xA4); // Output follows RAM content

    ssd1306_WriteCommand(hi2c, 0xD3); // Set display offset
    ssd1306_WriteCommand(hi2c, 0x00); // no offset

    ssd1306_WriteCommand(hi2c, 0xD5); // Set display clock divide ratio/oscillator frequency
    ssd1306_WriteCommand(hi2c, 0xF0); // set divide ratio

    ssd1306_WriteCommand(hi2c, 0xD9); // Set pre-charge period
    ssd1306_WriteCommand(hi2c, 0x22);

    ssd1306_WriteCommand(hi2c, 0xDA); // Set com pins hardware configuration
    ssd1306_WriteCommand(hi2c, 0x12);

    ssd1306_WriteCommand(hi2c, 0xDB); // Set vcomh
    ssd1306_WriteCommand(hi2c, 0x20); // 0.77xVcc

    ssd1306_WriteCommand(hi2c, 0x8D); // Set DC-DC enable
    ssd1306_WriteCommand(hi2c, 0x14);

    ssd1306_WriteCommand(hi2c, 0xAF); // Display ON

    // 3. Clear the screen initially
    memset(SSD1306_Buffer, 0, 1024);
    ssd1306_UpdateScreen(hi2c);
}



void ssd1306_SetCursor(uint8_t x, uint8_t y) {
    SSD1306.CurrentX = x;
    SSD1306.CurrentY = y;
}

char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, uint8_t color) {
    uint32_t i, b, j;

    if (128 <= (SSD1306.CurrentX + Font.width) ||
        64 <= (SSD1306.CurrentY + Font.height)) {
        return 0;
    }

    for (i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for (j = 0; j < Font.width; j++) {
            if ((b << j) & 0x8000) {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (uint8_t)color);
            } else {
                ssd1306_DrawPixel(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (uint8_t)!color);
            }
        }
    }

    SSD1306.CurrentX += Font.width;
    return ch;
}

char ssd1306_WriteString(char* str, SSD1306_Font_t Font, uint8_t color) {
    while (*str) {
        if (ssd1306_WriteChar(*str, Font, color) != *str) {
            return *str;
        }
        str++;
    }
    return *str;
}


