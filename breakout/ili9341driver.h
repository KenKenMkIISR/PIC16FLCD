#ifndef ILI9341DRIVER_H
#define	ILI9341DRIVER_H

#include <xc.h>
#include <stdint.h>
#include "mcc_generated_files/mcc.h"
#define X_RES 240
#define Y_RES 320

void LCD_Init();
void LCD_Clear(uint16_t color);
void LCD_SetCursor(uint16_t x, uint16_t y);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_WriteData2(uint16_t data);
void LCD_setAddrWindow(uint16_t x,uint16_t y,uint16_t w,uint16_t h);
void LCD_continuous_output(uint16_t x,uint16_t y,uint16_t color,uint16_t n);

#endif	/* ILI9341DRIVER_H */

