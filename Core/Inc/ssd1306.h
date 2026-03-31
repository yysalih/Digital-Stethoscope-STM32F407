/*
 * ssd1306.h
 *
 *  Created on: Dec 6, 2025
 *      Author: Salih
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#define SSD1306_I2C_ADDR	0x3C

#include <stdint.h>
#include "main.h"

typedef struct {
	const uint8_t width;                /**< Font width in pixels */
	const uint8_t height;               /**< Font height in pixels */
	const uint16_t *const data;         /**< Pointer to font data array */
    const uint8_t *const char_width;    /**< Proportional character width in pixels (NULL for monospaced) */
} SSD1306_Font_t;


typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

extern uint8_t SSD1306_Buffer[1024];

void ssd1306_SetCursor(uint8_t x, uint8_t y);
char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, uint8_t color);
char ssd1306_WriteString(char* str, SSD1306_Font_t Font, uint8_t color);



void ssd1306_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_UpdateScreen(I2C_HandleTypeDef *hi2c);
void ssd1306_WriteCommand(I2C_HandleTypeDef *hi2c, uint8_t data);
void ssd1306_Init(I2C_HandleTypeDef *hi2c);



#endif /* INC_SSD1306_H_ */
