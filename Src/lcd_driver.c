/***************************************************************************************************
  * @file       NucleoF446RE_LCD.h
	* @author     BFH-TI / wlm2
  * @author     BFH-TI / dnd1
	* @author     BFH-TI / knm7
  * @author     ?? original author has to be given as well !!	
  * @version    V0.4
	* @date       30.09.2018
	* @date       20.04.2018
	* @date       25.11.2017
  * @date       28.10.2015
  * @brief      Display setup and communication for LCD on mbed16-application-shield
	* @brief      V0.4;dnd1;add;drawLine() function
  * @hardware   NucleoF446RE with mbed-016.1-application-shield
  **************************************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2018 BFH-TI </center></h2>
  *
  */

#include "lcd_driver.h"
#include "lcd_symbols.h"
#include "lcd_font6x8.h"
#include "lcd_font16x24.h"
#include "stm32f4xx_hal.h"
#include "spi.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_ROWS      ((uint8_t) 128)
#define NUMBER_OF_COLUMNS   ((uint8_t) 32)


//$dnd no binary constants 0b.. neither in C99 nor current compiler
typedef enum{DISPLAY_OFF                = 0xAE, // 10101110,
             DISPLAY_ON                 = 0xAF, // 10101111,
             ADC_NORMAL                 = 0xA0, // 10100000,
             ADC_REVERSE                = 0xA1, // 10100001,
             BIAS_ONE_NINTH             = 0xA2, // 10100010,
             BIAS_ONE_SEVENTH           = 0xA3, // 10100011,
             COMMON_OUTPUT_MODE_NORMAL  = 0xC0, // 11000000,
             COMMON_OUTPUT_MODE_REVERSE = 0xC8, // 11001000,
             POWER_CONTROL_SET_0        = 0x28, // 00101000,
             POWER_CONTROL_SET_1        = 0x29, // 00101001,
             POWER_CONTROL_SET_3        = 0x2C, // 00101011,
             POWER_CONTROL_SET_7        = 0x2F, // 00101111,
             INTERNAL_RESISTOR_RATIO_0  = 0x20, // 00100000,
             INTERNAL_RESISTOR_RATIO_1  = 0x21, // 00100001,
             INTERNAL_RESISTOR_RATIO_2  = 0x22, // 00100010,
             INTERNAL_RESISTOR_RATIO_3  = 0x23, // 00100011,
             INTERNAL_RESISTOR_RATIO_4  = 0x24, // 00100100,
             INTERNAL_RESISTOR_RATIO_5  = 0x25, // 00100101,
             INTERNAL_RESISTOR_RATIO_6  = 0x26, // 00100110,
             INTERNAL_RESISTOR_RATIO_7  = 0x27, // 00100111,
             ELECTRONIC_VOLUME_MODE_SET = 0x81, // 10000001,
             PAGE_ADDRESS_SET_0         = 0xB0, // 10110000,
             PAGE_ADDRESS_SET_1         = 0xB1, // 10110001,
             PAGE_ADDRESS_SET_2         = 0xB2, // 10110010,
             PAGE_ADDRESS_SET_3         = 0xB3, // 10110011,
             COLUMN_ADDRESS_SET_BIT     = 0x10, // 00010000,
             BOOSTER_RATIO_SET          = 0x00  // 00000000
            }instruction;

typedef enum{PAGE_0, PAGE_1, PAGE_2, PAGE_3, NUMBER_OF_PAGES}pages;

typedef enum{INSTRUCTION_REGISTER, DATA_REGISTER}lcdRegister;

typedef struct FontSize_{
    uint8_t height;
    uint8_t width;
}FontSize;

static FontSize const fontSizes[LCD_NUMBER_OF_FONTS] = {{8, 6}, {24, 16}};


typedef uint8_t page; /* a page consists of a column of 8 rows */
typedef page pageSet[NUMBER_OF_PAGES]; /* a full column consists of 4 pages with 8 rows, 32 rows in total*/

typedef union ColumnBuffer_{
    uint32_t rawData;
    pageSet pageSet;
}ColumnBuffer;

static ColumnBuffer displayBuffer[NUMBER_OF_ROWS];



static void chipSelect(bool isSelected)
{
    HAL_GPIO_WritePin(LCD_CS_N_GPIO_Port, LCD_CS_N_Pin, (GPIO_PinState) !isSelected); /* low active */
}

static void registerSelect(lcdRegister selectedRegister)
{
    HAL_GPIO_WritePin(LCD_A0_GPIO_Port, LCD_A0_Pin, (GPIO_PinState) selectedRegister);
}

static void displayReset(void)
{
    chipSelect(INSTRUCTION_REGISTER);
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_RESET_GPIO_Port, LCD_RESET_Pin, GPIO_PIN_SET);
    chipSelect(DATA_REGISTER);
}

static void sendInstruction(instruction command)
{
    registerSelect(INSTRUCTION_REGISTER);
    chipSelect(true);
	  //$dnd (uint8_t) added
    HAL_SPI_Transmit(&hspi1, (uint8_t *) &command, 1, 1);
    chipSelect(false);
}


/*static*/ void sendData(uint8_t data)
{
    registerSelect(DATA_REGISTER);
    chipSelect(true);
    HAL_SPI_Transmit(&hspi1, &data, 1, 1);
    chipSelect(false);
}

void lcd_setContrast(uint8_t electronicVolume)
{
#define ELECTRONIC_VOLUME_MAXIMUM_RAITING ((uint8_t) 63)

    if (electronicVolume <= ELECTRONIC_VOLUME_MAXIMUM_RAITING) {
        sendInstruction(ELECTRONIC_VOLUME_MODE_SET);
			//$dnd1 added (instruction)
        sendInstruction((instruction) electronicVolume);
    }
}

void lcd_init(void)
{
    lcd_clear();
    displayReset();

    sendInstruction(ADC_NORMAL);
    sendInstruction(DISPLAY_OFF);
    sendInstruction(COMMON_OUTPUT_MODE_REVERSE);
    sendInstruction(BIAS_ONE_NINTH);
    sendInstruction(POWER_CONTROL_SET_7);
    sendInstruction(INTERNAL_RESISTOR_RATIO_1);
    lcd_setContrast(40);
    sendInstruction(DISPLAY_ON);
}

void lcd_setPixel(uint8_t xPosition, uint8_t yPosition, bool pixelIsSet)
{
    if ((xPosition < NUMBER_OF_ROWS) && (yPosition < NUMBER_OF_COLUMNS)) {
        if (pixelIsSet)
            displayBuffer[xPosition].rawData |= (1 << yPosition);
        else
            displayBuffer[xPosition].rawData &= ~(1 << yPosition);
    }
}

void lcd_clear(void)
{
    for (uint8_t rowIndex = 0; rowIndex < NUMBER_OF_ROWS; rowIndex++) {
        displayBuffer[rowIndex].rawData = false;
    }
}


void lcd_setChar(uint8_t xPosition, uint8_t yPosition, unsigned char charToSet, lcd_fontSize size, bool contrastIsInverted)
{
#define ASCII_TABLE_OFFSET ((uint8_t) 32)

    charToSet -= ASCII_TABLE_OFFSET;

    for (uint8_t columnCounter = 0; columnCounter < fontSizes[size].height; columnCounter++)
        for (uint8_t rowCounter = 0; rowCounter < fontSizes[size].width; rowCounter++){
            if (Font_6x8_h[charToSet * fontSizes[size].height + columnCounter] & (1 << rowCounter))
                lcd_setPixel(xPosition + rowCounter, yPosition + columnCounter, !contrastIsInverted);
            else
                lcd_setPixel(xPosition + rowCounter, yPosition + columnCounter, contrastIsInverted);
        }
}




void lcd_setString(uint8_t xPosition, uint8_t yPosition, char const * string, lcd_fontSize size, bool contrastIsInverted )
{
 if ((size <= LCD_FONT_8) && (size < LCD_NUMBER_OF_FONTS))
 //$dnd1 bug this works only for font enum = 0. No large fonts!
        for(; *string != '\0'; string++){
            lcd_setChar(xPosition, yPosition, *string, size, contrastIsInverted);
            xPosition += fontSizes[size].width;
        }
}

// V0.4;dnd1;add;drawLine() function

void lcd_setLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t state)
{
	int16_t i, x[2], y[2];
	float xF, yF;

	xF = x1, x[1] = 0;
	yF = y1, y[1] = 0;

	x[0] = abs(x2-x1);
	y[0] = abs(y2-y1);

	if(x[0]) x[1] = (x2-x1)/x[0];
	if(y[0]) y[1] = (y2-y1)/y[0];

	if(x[0] >= y[0]) {
		for(i = 0; i<x[0]; i++) {
			if( xF >= 0 && yF >= 0) lcd_setPixel(xF, yF, state);
			xF += x[1];
			yF += ((float)y[0]/x[0])*y[1];
		}
	}
	else {
		for(i = 0; i<y[0]; i++) {
			if( xF >= 0 && yF >= 0) lcd_setPixel(xF, yF, state);
			xF += ((float)x[0]/y[0])*x[1];
			yF += y[1];
		}
	}
}

void lcd_show(void){
    for (pages pageIndex = PAGE_0; pageIndex < NUMBER_OF_PAGES; pageIndex++){
			  //$dnd added (instruction)
        sendInstruction((instruction) (PAGE_ADDRESS_SET_0 + pageIndex));
        sendInstruction(COLUMN_ADDRESS_SET_BIT);
        sendInstruction(BOOSTER_RATIO_SET);
        for (uint8_t rowIndex = 0; rowIndex < NUMBER_OF_ROWS; rowIndex++)
            sendData(displayBuffer[rowIndex].pageSet[pageIndex]);
    }
}

void lcd_setFrame(uint8_t xPositionUL, uint8_t yPositionUL, uint8_t xPositionDR, uint8_t yPositionDR)
{
    for (uint8_t rowCounter = yPositionUL; rowCounter <= yPositionDR; rowCounter++){
        lcd_setPixel(xPositionUL, rowCounter, true);
        lcd_setPixel(xPositionDR, rowCounter, true);
    }

    for (uint8_t columnCounter = xPositionUL; columnCounter <= xPositionDR; columnCounter++){
        lcd_setPixel(columnCounter, yPositionDR, true);
        lcd_setPixel(columnCounter, yPositionUL, true);
    }
}

void lcd_setBar(uint8_t xPositionUL, uint8_t yPositionUL, uint8_t xPositionDR, uint8_t yPositionDR)
{
    for (uint8_t rowCounter = yPositionUL; rowCounter <= yPositionDR; rowCounter++)
        for (uint8_t columnCounter = xPositionUL; columnCounter <= xPositionDR; columnCounter++)
            lcd_setPixel(columnCounter, rowCounter, true);

}

void lcd_setSymbol8(uint8_t xPosition, uint8_t yPosition, lcd_symbol symbol, bool contrastIsInverted)
{
    for (uint8_t columnCounter = 0; columnCounter < fontSizes[LCD_FONT_8].height ; columnCounter++)
        for (uint8_t rowCounter = 0; rowCounter < fontSizes[LCD_FONT_8].width; rowCounter++){
            if (lcd_symbols[symbol * fontSizes[LCD_FONT_8].height + columnCounter] & (1 << rowCounter))
                lcd_setPixel(xPosition + rowCounter, yPosition + columnCounter, !contrastIsInverted);
            else
                lcd_setPixel(xPosition + rowCounter, yPosition + columnCounter, contrastIsInverted);
        }
}

void lcd_setBackgroundLight(bool value)
{
	if (value) {
	  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);
	}
};


/****** END OF FILE ******/
