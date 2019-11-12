#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum{LCD_FONT_8, LCD_FONT_24, LCD_NUMBER_OF_FONTS}lcd_fontSize;
typedef enum{LCD_MENU_SYMBOL, LCD_PLAY_SYMBOL, LCD_NUMBER_OF_SYMBOLS}lcd_symbol;

void lcd_init(void);
void lcd_clear(void);
void lcd_show(void);

void lcd_setString(uint8_t x, uint8_t y, const char *s, uint8_t size , bool contrastIsInverted);
void lcd_setPixel(uint8_t xPosition, uint8_t yPosition, bool pixelIsSet);
void lcd_setContrast(uint8_t electronicVolume);
void lcd_setFrame(uint8_t xPositionUL, uint8_t yPositionUL, uint8_t xPositionDR, uint8_t yPositionDR);
void lcd_setBar(uint8_t xPositionUL, uint8_t yPositionUL, uint8_t xPositionDR, uint8_t yPositionDR);
void lcd_setSymbol8(uint8_t xPosition, uint8_t yPosition, lcd_symbol symbol, bool contrastIsInverted);

void lcd_setLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t state);
void lcd_setBackgroundLight(bool value);

void sendData(uint8_t data);


#ifdef __cplusplus
}
#endif

#endif /* LCD_DRIVER_H */
