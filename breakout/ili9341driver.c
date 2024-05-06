#include "ili9341driver.h"

#define lcd_dc_lo LATA=LATA & 0xfb
#define lcd_dc_hi LATA=LATA | 0x04
#define lcd_reset_lo LATA=LATA & 0xef
#define lcd_reset_hi LATA=LATA | 0x10

void delayms(uint16_t t);

void LCD_WriteComm(uint8_t comm){
// Write Command
	lcd_dc_lo;
	SPI1_Exchange8bit(comm);
}

void LCD_WriteData(uint8_t data)
{
// Write Data
	lcd_dc_hi;
	SPI1_Exchange8bit(data);
}

void LCD_WriteData2(uint16_t data)
{
// Write Data 2 bytes
	lcd_dc_hi;
	SPI1_Exchange8bit(data>>8);
	SPI1_Exchange8bit((uint8_t)data);
}

void LCD_Init()
{
	lcd_dc_hi;

	// Reset controller
	lcd_reset_hi;
	delayms(1);
	lcd_reset_lo;
	delayms(10);
	lcd_reset_hi;
	delayms(120);

	LCD_WriteComm(0x36);
	LCD_WriteData(0x48); //Vertical
	LCD_WriteComm(0x3A);
	LCD_WriteData(0x55);
 	LCD_WriteComm(0x11);
	delayms(120);
	LCD_WriteComm(0x29);
}

void LCD_setAddrWindow(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
{
	LCD_WriteComm(0x2a);
	LCD_WriteData2(x);
	LCD_WriteData2(x+w-1);
	LCD_WriteComm(0x2b);
	LCD_WriteData2(y);
	LCD_WriteData2(y+h-1);
	LCD_WriteComm(0x2c);
}

void LCD_SetCursor(uint16_t x, uint16_t y)
{
	LCD_setAddrWindow(x,y,X_RES-x,1);
}

void LCD_continuous_output(uint16_t x,uint16_t y,uint16_t color,uint16_t n)
{
	//High speed continuous output
	uint16_t i;
	LCD_setAddrWindow(x,y,n,1);
	lcd_dc_hi;
	for (i=0; i < n ; i++){
		SPI1_Exchange8bit(color>>8);
		SPI1_Exchange8bit((uint8_t)color);
	}
}
void LCD_Clear(uint16_t color)
{
	uint16_t i;
	for (i=0; i < Y_RES ; i++){
		LCD_continuous_output(0,i,color,X_RES);
	}
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_SetCursor(x,y);
	LCD_WriteData2(color);
}
